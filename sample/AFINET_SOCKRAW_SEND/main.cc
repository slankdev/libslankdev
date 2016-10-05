
#include <stdio.h>
#include <slankdev.h>
#include <arpa/inet.h>
using namespace slankdev;


void sendpacket_l3(const void* buf, size_t buflen,
        const char* device, struct sockaddr_in* dst, size_t dstlen)
{
    socketfd fd;
    fd.socket(AF_INET, SOCK_RAW, IPPROTO_RAW);

    struct ifreq ifr;
    memset(&ifr, 0, sizeof ifr);
    snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "%s", device);
    fd.ioctl(SIOCGIFINDEX, &ifr);

    uint32_t on = 1;
    fd.setsockopt(IPPROTO_IP, IP_HDRINCL, &on, sizeof(on));
    fd.setsockopt(SOL_SOCKET, SO_BINDTODEVICE, &ifr, sizeof ifr);

    fd.sendto(buf, buflen, 0, (struct sockaddr*)dst, dstlen);
    printf("send packet\n");
}


int main(int argc, char** argv)
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s interface destination\n",argv[0]);
        return -1;

    }

    size_t udpdata_len = 10000;
    
    uint8_t buf[100000];
    memset(buf, 0xee, sizeof buf);

    struct udp* uh = reinterpret_cast<struct udp*>(buf + sizeof(ip));
    uh->src      = ::htons(1111);
    uh->dst      = ::htons(1111);
    uh->len      = ::htons(8 + udpdata_len);
    uh->checksum = ::htons(0x00);

    struct ip* iph = reinterpret_cast<struct ip*>(buf);
    iph->ihl = 5;
    iph->version = 4;
    iph->off = ::htons(uint16_t(0x4000));
    iph->len = sizeof(iph) + ::ntohs(uh->len);
    iph->ttl = 64;
    iph->protocol = 0x11; // ICMP
    iph->sum = 0;
    iph->src.s_addr = inet_addr("192.168.222.100");
    iph->dst.s_addr = inet_addr(argv[2]);
    iph->sum = checksum(&iph, sizeof iph);

    struct sockaddr_in dst;
    memset(&dst, 0, sizeof dst);
    dst.sin_family = AF_INET;
    dst.sin_addr.s_addr = iph->dst.s_addr;

    size_t  buflen = sizeof(struct ip) + ::ntohs(uh->len);
    sendpacket_l3(buf, buflen, argv[1], &dst, sizeof(dst));
}


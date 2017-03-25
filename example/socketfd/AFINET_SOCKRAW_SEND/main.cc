
#include <stdio.h>
#include <slankdev.h>

namespace s = slankdev;


void sendpacket_l3(const void* buf, size_t buflen,
        const char* device, struct sockaddr_in* dst, size_t dstlen)
{
    s::socketfd fd;
    fd.socket(AF_INET, SOCK_RAW, 255);

    struct ifreq ifr;
    memset(&ifr, 0, sizeof ifr);
    snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "%s", device);
    fd.ioctl(SIOCGIFINDEX, &ifr);

    uint32_t val = 0;
    fd.setsockopt(IPPROTO_IP, IP_HDRINCL, &val, sizeof(val));
    val = 1;
    fd.setsockopt(IPPROTO_IP, IP_MTU_DISCOVER, &val, sizeof(val));
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

    uint8_t buf[100000];
    memset(buf, 0xee, sizeof buf);
    size_t data_len = 10000;

    struct sockaddr_in dst;
    memset(&dst, 0, sizeof dst);
    dst.sin_family = AF_INET;
    dst.sin_port = s::htons(1111);
    dst.sin_addr.s_addr = inet_addr(argv[2]);

    sendpacket_l3(buf, data_len, argv[1], &dst, sizeof(dst));
}

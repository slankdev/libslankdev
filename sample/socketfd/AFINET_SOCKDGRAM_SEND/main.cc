
#include <stdio.h>
#include <slankdev.h>
#include <arpa/inet.h>
using namespace slankdev;


void sendpacket_l3(const void* buf, size_t buflen,
        const char* device, struct sockaddr_in* dst, size_t dstlen)
{
    socketfd fd;
    fd.socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    struct ifreq ifr;
    memset(&ifr, 0, sizeof ifr);
    snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "%s", device);
    fd.ioctl(SIOCGIFINDEX, &ifr);

    fd.sendto(buf, buflen, 0, (struct sockaddr*)dst, dstlen);
    printf("send packet. len=%zd\n", buflen);
}


int main(int argc, char** argv)
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s interface destination\n",argv[0]);
        return -1;

    }

    size_t datalen = 10000;
    uint8_t buf[100000];
    memset(buf, 0xee, sizeof buf);

    struct sockaddr_in dst;
    memset(&dst, 0, sizeof dst);
    dst.sin_family = AF_INET;
    dst.sin_port = ::htons(1111);
    dst.sin_addr.s_addr = inet_addr(argv[2]);

    sendpacket_l3(buf, datalen, argv[1], &dst, sizeof(dst));
}


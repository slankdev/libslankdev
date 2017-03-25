
#include <stdio.h>
#include <slankdev.h>

#if 0
# define PROTOCOL IPPROTO_TCP
#else
# define PROTOCOL IPPROTO_ICMP
#endif

using namespace slankdev;

int main(int argc, char** argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s interface\n",argv[0]);
        return -1;

    }

    socketfd fd;
    fd.socket(AF_INET, SOCK_RAW, PROTOCOL);

    struct ifreq ifr;
    memset(&ifr, 0, sizeof ifr);
    snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "%s", argv[1]);
    fd.ioctl(SIOCGIFINDEX, &ifr);

    uint32_t on = 1;
    fd.setsockopt(IPPROTO_IP, IP_HDRINCL, &on, sizeof(on));
    fd.setsockopt(SOL_SOCKET, SO_BINDTODEVICE, &ifr, sizeof ifr);

    while (1) {
        uint8_t buf[10000];
        memset(buf, 0, sizeof buf);
        struct sockaddr_in src;
        socklen_t srclen = sizeof(src);
        memset(&src, 0, srclen);

        size_t recvlen = fd.recvfrom(buf, sizeof buf, 0, (struct sockaddr*)&src, &srclen);
        hexdump("Recieve Packet", buf, recvlen);
    }
}


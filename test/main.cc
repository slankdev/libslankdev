
#include <slankdev.h>

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <slankdev.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <net/if.h>

#include <netpacket/packet.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>
#include <sys/socket.h>


const char* dev = "eth0";
const char* dev1 = "enp0s8";
const char* dev2 = "enp0s9";

int main()
{
    base b;
    b.add_if(dev1);
    b.add_if(dev2);
    
    while (1) {
        std::string name;
        uint8_t buf[10000];
        size_t recvlen = b.recv_any(name, buf, sizeof buf);

        printf("%s recvlen=%zu \n", name.c_str(), recvlen);
    }
}


#if 0
static uint8_t raw[] = { 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x09, 0x0f, 0x09, 0x00, 0x0d, 0x08, 0x06, 0x00, 0x01,
    0x08, 0x00, 0x06, 0x04, 0x00, 0x01, 0x00, 0x09, 0x0f, 0x09, 0x00, 0x0d, 0x0a, 0xd2, 0x7c, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0xd2, 0x7c, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

int gfd;

void func()
{
    safe_intfd fd;
    fd.socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    struct ifreq ifreq;
    memset(&ifreq, 0, sizeof(ifreq));
    strncpy(ifreq.ifr_name, dev, sizeof(ifreq.ifr_name)-1);
    fd.ioctl(SIOCGIFINDEX, &ifreq);

    struct sockaddr_ll sa;
    sa.sll_family = AF_PACKET;
    sa.sll_protocol = htonl(ETH_P_ALL);
    sa.sll_ifindex = ifreq.ifr_ifindex;
    fd.bind((struct sockaddr*)&sa, sizeof(sa));

    fd.ioctl(SIOCGIFFLAGS, &ifreq);
    ifreq.ifr_flags = ifreq.ifr_flags | IFF_PROMISC;
    fd.ioctl(SIOCSIFFLAGS, &ifreq);
    
    fd.write(raw, sizeof(raw));
}

int main()
{
    gfd= -1;
    func();
    write(gfd, raw, sizeof(raw));
}

#endif

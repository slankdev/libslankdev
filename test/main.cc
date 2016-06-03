
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


const char* dev1 = "enp0s8";
const char* dev2 = "enp0s9";

using namespace slankdev;


static safe_intfd* open_if(const std::string& name)
{
    safe_intfd *fd = new safe_intfd();
    fd->socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    struct ifreq ifreq;
    memset(&ifreq, 0, sizeof(ifreq));
    strncpy(ifreq.ifr_name, name.c_str(), sizeof(ifreq.ifr_name)-1);
    fd->ioctl(SIOCGIFINDEX, &ifreq);

    struct sockaddr_ll sa;
    sa.sll_family = PF_PACKET;
    sa.sll_protocol = htonl(ETH_P_ALL);
    sa.sll_ifindex = ifreq.ifr_ifindex;
    fd->bind((struct sockaddr*)&sa, sizeof(sa));

    fd->ioctl(SIOCGIFFLAGS, &ifreq);
    ifreq.ifr_flags = ifreq.ifr_flags | IFF_PROMISC;
    fd->ioctl(SIOCSIFFLAGS, &ifreq);

    return fd;
}


int main()
{
    uint8_t raw[64];
    memset(raw, 0xee, sizeof raw);

    safe_intfd* fd = open_if(dev1);
    fd->write(raw, sizeof(raw));
    delete fd;
}



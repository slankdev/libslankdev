

#include <slankdev/intfd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <net/if.h>

#include <netpacket/packet.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>
#include <sys/socket.h>


namespace slankdev {




unsafe_intfd::unsafe_intfd() : fd(-1) {}


void unsafe_intfd::socket(int domain, int type, int protocol)
{
    fd = ::socket(domain, type, protocol);
    if (fd < 0) {
        perror("socket");
        exit(-1);
    }
}


void unsafe_intfd::open(const char* path, int flags)
{
    fd = ::open(path, flags);
    if (fd < 0) {
        perror("open");
        exit(-1);
    }
}


void unsafe_intfd::close()
{
    if (fd >= 0)
        ::close(fd);
}


void unsafe_intfd::bind(const struct sockaddr* sa, size_t len)
{
    int res = ::bind(fd, sa, len);
    if (res < 0) {
        perror("bind");
        exit(-1);
    }
}


void unsafe_intfd::ioctl(unsigned long l, void* arg)
{
    int res = ::ioctl(fd, l, arg);
    if (res < 0) {
        perror("ioctl");
        exit(-1);
    }
}


void unsafe_intfd::write(const void* buffer, size_t bufferlen)
{
    ssize_t res = ::write(fd, buffer, bufferlen);
    if (static_cast<size_t>(res) != bufferlen) {
        if (res < 0) {
            perror("write");
            exit(-1);
        } else {
            fprintf(stderr, "write could not send all.\n");
        } 
    }
}


size_t unsafe_intfd::read(void* buffer, size_t bufferlen)
{
    ssize_t res = ::read(fd, buffer, bufferlen);
    if (res < 0) {
        perror("read");
        exit(-1);
    } else if (res == EINTR) {
        return read(buffer, bufferlen);
    }
    return static_cast<size_t>(res);
}



void unsafe_intfd::open_if(const char* name)
{
    socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    
    struct ifreq ifreq;
    memset(&ifreq, 0, sizeof(ifreq));
    strncpy(ifreq.ifr_name, name, sizeof(ifreq.ifr_name)-1);
    ioctl(SIOCGIFINDEX, &ifreq);

    struct sockaddr_ll sa;
    sa.sll_family = PF_PACKET;
    sa.sll_protocol = htonl(ETH_P_ALL);
    sa.sll_ifindex = ifreq.ifr_ifindex;
    bind((struct sockaddr*)&sa, sizeof(sa));

    ioctl(SIOCGIFFLAGS, &ifreq);
    ifreq.ifr_flags = ifreq.ifr_flags | IFF_PROMISC;
    ioctl(SIOCSIFFLAGS, &ifreq);
}




safe_intfd::~safe_intfd()
{
    close();
}


} /* namespace slankdev */

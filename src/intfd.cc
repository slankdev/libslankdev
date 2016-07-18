

#include <slankdev/intfd.h>
#include <slankdev/system.h>

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




void unsafe_intfd::open(const char* path, int flags)
{
    fd = ::open(path, flags);
    if (fd < 0) {
        perror("open");
        exit(-1);
    }
}



void unsafe_intfd::open(const char* path, int flags, mode_t mode)
{
    fd = ::open(path, flags, mode);
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
    printf("fd %d \n", fd);
    ssize_t res = ::read(fd, buffer, bufferlen);
    if (res < 0) {
        perror("read");
        exit(-1);
    } else if (res == EINTR) {
        return read(buffer, bufferlen);
    }
    return static_cast<size_t>(res);
}








safe_intfd::~safe_intfd()
{
    close();
}


int safe_intfd::get_fd()
{
    return fd;
}



void safe_intfd::set_fd(int f)
{
    fd = f;
}


void socketfd::socket(int domain, int type, int protocol)
{
    fd = ::socket(domain, type, protocol);
    if (fd < 0) {
        perror("socket");
        exit(-1);
    }
}
void socketfd::bind(const struct sockaddr* sa, size_t len)
{
    int res = ::bind(fd, sa, len);
    if (res < 0) {
        perror("bind");
        exit(-1);
    }
}
void socketfd::listen(int backlog)
{
    int res = ::listen(fd, backlog);
    if (res < 0) {
        perror("listen");
        exit(-1);
    }
}
int socketfd::accept(struct sockaddr* sa, socklen_t* len)
{
    int res = ::accept(fd, sa, len);
    if (res < 0) {
        perror("accept");
        exit(-1);
    }
    return res;
}
void socketfd::sendto(const void* buffer, size_t bufferlen,int flags, 
        const struct sockaddr* dest_addr, socklen_t dest_len)
{
    ssize_t res = ::sendto(fd, buffer, bufferlen, flags, dest_addr, dest_len);
    if (res < 0) {
        perror("sendto");
        exit(-1);
    }
}


size_t socketfd::recvfrom(void* buffer, size_t bufferlen, int flags,
        struct sockaddr* address, socklen_t* address_len)
{
    ssize_t res = ::recvfrom(fd, buffer, bufferlen, flags, address, address_len);
    if (res < 0) {
        perror("recvfrom");
        exit(-1);
    }
}



void socketfd::getsockopt(int level, int optname, void* optval, socklen_t *optlen)
{
    int res = ::getsockopt(fd, level, optname, optval, optlen);
    if (res < 0) {
        perror("getsockopt");
        exit(-1);
    }
}
void socketfd::setsockopt(int level, int optname, const void* optval, socklen_t optlen)
{
    int res = ::setsockopt(fd, level, optname, optval, optlen);
    if (res < 0) {
        perror("setsockopt");
        exit(-1);
    }
}



#ifdef __linux__
void socketfd::open_if(const char* name)
{
    socket(PF_PACKET, SOCK_RAW, slankdev::htons(ETH_P_ALL));
    
    struct ifreq ifreq;
    memset(&ifreq, 0, sizeof(ifreq));
    strncpy(ifreq.ifr_name, name, sizeof(ifreq.ifr_name)-1);
    ioctl(SIOCGIFINDEX, &ifreq);

    struct sockaddr_ll sa;
    sa.sll_family = PF_PACKET;
    sa.sll_protocol = slankdev::htons(ETH_P_ALL);
    sa.sll_ifindex = ifreq.ifr_ifindex;
    bind((struct sockaddr*)&sa, sizeof(sa));

    ioctl(SIOCGIFFLAGS, &ifreq);
    ifreq.ifr_flags = ifreq.ifr_flags | IFF_PROMISC;
    ioctl(SIOCSIFFLAGS, &ifreq);
}
#endif

} /* namespace slankdev */

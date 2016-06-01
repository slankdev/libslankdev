

#include <slankdev/intfd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
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





impl_intfd::impl_intfd() : _fd(-1) {}


void impl_intfd::socket(int domain, int type, int protocol)
{
    _fd = ::socket(domain, type, protocol);
    if (_fd < 0) {
        perror("socket");
        exit(-1);
    }
}


void impl_intfd::open(const char* path, int flags)
{
    _fd = ::open(path, flags);
    if (_fd < 0) {
        perror("open");
        exit(-1);
    }
}


void impl_intfd::close()
{
    if (_fd >= 0)
        ::close(_fd);
}


void impl_intfd::bind(const struct sockaddr* sa, size_t len)
{
    int res = ::bind(_fd, sa, len);
    if (res < 0) {
        perror("bind");
        exit(-1);
    }
}


void impl_intfd::ioctl(unsigned long l, void* arg)
{
    int res = ::ioctl(_fd, l, arg);
    if (res < 0) {
        perror("ioctl");
        exit(-1);
    }
}


void impl_intfd::write(const void* buffer, size_t bufferlen)
{
    ssize_t res = ::write(_fd, buffer, bufferlen);
    if (res < 0) {
        perror("write");
        exit(-1);
    } else if (static_cast<size_t>(res) != bufferlen) {
        fprintf(stderr, "write could not send all.\n");
    } 
}


size_t impl_intfd::read(void* buffer, size_t bufferlen)
{
    ssize_t res = ::read(_fd, buffer, bufferlen);
    if (res < 0) {
        perror("read");
        exit(-1);
    }
}





int unsafe_intfd::fd()
{
    return this->_fd;
}



safe_intfd::~safe_intfd()
{
    close();
}


#pragma once

#include <slankdev/system.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <net/if.h>
#include <sys/socket.h> // for struct sockaddr
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>

#include <netinet/in.h> // for IPPROTO_**
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>

#ifdef __linux__
#include <netpacket/packet.h>
#endif



namespace slankdev {


#ifndef ETH_P_ALL
#define ETH_P_ALL  0x0003
#endif
#ifndef ETH_P_IP
#define ETH_P_IP   0x0800
#endif
#ifndef ETH_P_ARP
#define ETH_P_ARP  0x0806
#endif
#ifndef ETH_P_RARP
#define ETH_P_RARP 0x0835
#endif

#ifndef IPPROTO_ICMP
#define IPPROTO_ICMP 1
#endif
#ifndef IPPROTO_TCP
#define IPPROTO_TCP  6
#endif
#ifndef IPPROTO_UDP
#define IPPROTO_UDP  17
#endif
#ifndef IPPROTO_RAW
#define IPPROTO_RAW  255
#endif


void string2binary_in4(const char* src, struct ::sockaddr_in* dst)
{
    int res = inet_pton(AF_INET, src, &dst->sin_addr);
    if (res != 1) {
        perror("inet_pton");
        exit(-1);
    }
}


void binary2string_in4(const struct sockaddr_in* src, char* dst, size_t dstlen)
{
    const char* res = inet_ntop(AF_INET, &src->sin_addr, dst, dstlen);
    if (res == NULL) {
        perror("inet_ntop");
        exit(-1);
    }
}




class unsafe_intfd {
    protected:
        int fd;
    public:
        int get_fd()
        {
            return fd;
        }
        void set_fd(int f)
        {
            if (f >= 0)
                close();
            fd = f;
        }

        unsafe_intfd() : fd(-1) {}
        void open(const char* path, int flags)
        {
            fd = ::open(path, flags);
            if (fd < 0) {
                perror("open");
                exit(-1);
            }
        }
        void open(const char* path, int flags, mode_t mode)
        {
            fd = ::open(path, flags, mode);
            if (fd < 0) {
                perror("open");
                exit(-1);
            }
        }
        void close()
        {
            if (fd >= 0)
                ::close(fd);
        }
        void ioctl(unsigned long l, void* arg)
        {
            int res = ::ioctl(fd, l, arg);
            if (res < 0) {
                perror("ioctl");
                exit(-1);
            }
        }
        void write(const void* buffer, size_t bufferlen)
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
        size_t read(void* buffer, size_t bufferlen)
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

        /* These don't work correctly */
        // template<typename... ARG>
        // void printf(const char* const fmt, const ARG&... arg)
        // {
        //     ::dprintf(fd, fmt, arg...);
        // }
        // void printf(const char* fmt)
        // {
        //     ::dprintf(fd, "%s", fmt);
        // }
};


class safe_intfd : public unsafe_intfd {
    public:
        ~safe_intfd()
        {
            close();
        }
};


class socketfd : public safe_intfd {
    public:
        void socket(int domain, int type, int protocol)
        {
            fd = ::socket(domain, type, protocol);
            if (fd < 0) {
                perror("socket");
                exit(-1);
            }
        }
        void bind(const struct sockaddr* sa, size_t len)
        {
            int res = ::bind(fd, sa, len);
            if (res < 0) {
                perror("bind");
                exit(-1);
            }
        }
        void listen(int backlog)
        {
            int res = ::listen(fd, backlog);
            if (res < 0) {
                perror("listen");
                exit(-1);
            }
        }
        int  accept(struct sockaddr* sa, socklen_t* len)
        {
            int res = ::accept(fd, sa, len);
            if (res < 0) {
                perror("accept");
                exit(-1);
            }
            return res;
        }
        void sendto(const void* buffer, size_t bufferlen,int flags,
                const struct sockaddr* dest_addr, socklen_t dest_len)
        {
            ssize_t res = ::sendto(fd, buffer, bufferlen, flags, dest_addr, dest_len);
            if (res < 0) {
                perror("sendto");
                exit(-1);
            }
        }
        size_t recvfrom(void* buffer, size_t bufferlen, int flags,
                struct sockaddr* address, socklen_t* address_len)
        {
            ssize_t res = ::recvfrom(fd, buffer, bufferlen, flags, address, address_len);
            if (res < 0) {
                perror("recvfrom");
                exit(-1);
            }
            return res;
        }
        void getsockopt(int level, int optname, void* optval, socklen_t *optlen)
        {
            int res = ::getsockopt(fd, level, optname, optval, optlen);
            if (res < 0) {
                perror("getsockopt");
                exit(-1);
            }
        }
        void setsockopt(int level, int optname, const void* optval, socklen_t optlen)
        {
            int res = ::setsockopt(fd, level, optname, optval, optlen);
            if (res < 0) {
                perror("setsockopt");
                exit(-1);
            }
        }
        void send(const void* buf, size_t nbyte, int flags)
        {
            int res = ::send(fd, buf, nbyte, flags);
            if (res < 0) {
                perror("send");
                exit(-1);
            }
        }
        size_t recv(void* buf, size_t nbyte, int flags)
        {
            ssize_t res = ::recv(fd, buf, nbyte, flags);
            if (res < 0) {
                perror("recv");
                exit(-1);
            }
            return res;
        }

#ifdef __linux__
        void open_if(const char* name)
        {
            socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

            struct ifreq ifreq;
            memset(&ifreq, 0, sizeof(ifreq));
            strncpy(ifreq.ifr_name, name, sizeof(ifreq.ifr_name)-1);
            ioctl(SIOCGIFINDEX, &ifreq);

            struct sockaddr_ll sa;
            sa.sll_family = PF_PACKET;
            sa.sll_protocol = htons(ETH_P_ALL);
            sa.sll_ifindex = ifreq.ifr_ifindex;
            bind((struct sockaddr*)&sa, sizeof(sa));

            ioctl(SIOCGIFFLAGS, &ifreq);
            ifreq.ifr_flags = ifreq.ifr_flags | IFF_PROMISC;
            ioctl(SIOCSIFFLAGS, &ifreq);
        }
#endif
};



} /* namespace slankdev */


#pragma once

#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <net/if.h>
#include <sys/socket.h> // for struct sockaddr
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>

#include <netinet/in.h> // for IPPROTO_**



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





class unsafe_intfd {
    protected:
        int fd;
    public:
        int get_fd();
        void set_fd(int f);

        unsafe_intfd();
        void open(const char* path, int flags);
        void open(const char* path, int flags, mode_t mode);
        void close();
        void ioctl(unsigned long l, void* arg);
        void write(const void* buffer, size_t bufferlen);
        size_t read(void* buffer, size_t bufferlen);

        /* These don't work correctry */
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
        ~safe_intfd();
};


class socketfd : public safe_intfd {
    public:
        void socket(int domain, int type, int protocol);
        void bind(const struct sockaddr* sa, size_t len);
        void listen(int backlog);
        int  accept(struct sockaddr* sa, socklen_t* len);
        void sendto(const void* buffer, size_t bufferlen,int flags, 
                const struct sockaddr* dest_addr, socklen_t dest_len);
        size_t recvfrom(void* buffer, size_t bufferlen, int flags,
                struct sockaddr* address, socklen_t* address_len);
        void getsockopt(int level, int optname, void* optval, socklen_t *optlen);
        void setsockopt(int level, int optname, const void* optval, socklen_t optlen);
        void send(const void* buf, size_t nbyte, int flags);
        size_t recv(void* buf, size_t nbyte, int flags);

#ifdef __linux__
        void open_if(const char* name);
#endif
};



} /* namespace slankdev */

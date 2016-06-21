
#pragma once

#include <stdio.h>
#include <stdint.h>
#include <sys/socket.h> // for struct sockaddr
#include <sys/stat.h>
#include <fcntl.h>

#include <netinet/in.h> // for IPPROTO_**


namespace slankdev {


#define ETH_P_ALL  0x0003
#define ETH_P_IP   0x0800
#define ETH_P_ARP  0x0806
#define ETH_P_RARP 0x0835





class unsafe_intfd {
    public:
        int fd;

        unsafe_intfd();
        void socket(int domain, int type, int protocol);
        void open(const char* path, int flags);
        void open(const char* path, int flags, mode_t mode);
        void close();
        void bind(const struct sockaddr* sa, size_t len);
        void ioctl(unsigned long l, void* arg);
        void write(const void* buffer, size_t bufferlen);
        size_t read(void* buffer, size_t bufferlen);

        void sendto(const void* buffer, size_t bufferlen,int flags, 
                const struct sockaddr* dest_addr, socklen_t dest_len);
        size_t recvfrom(void* buffer, size_t bufferlen, int flags,
                struct sockaddr* address, socklen_t* address_len);

        void open_if(const char* name);
};


class safe_intfd : public unsafe_intfd {
    private:
        int fd;
    public:
        ~safe_intfd();
};



} /* namespace slankdev */


#pragma once

#include <stdio.h>
#include <stdint.h>
#include <sys/socket.h> // for struct sockaddr



class impl_intfd {
    protected:
        int _fd;

    public:
        impl_intfd();

        void socket(int domain, int type, int protocol);
        void open(const char* path, int flags);
        void close();
        void bind(const struct sockaddr* sa, size_t len);
        void ioctl(unsigned long l, void* arg);
        void write(const void* buffer, size_t bufferlen);
        size_t read(void* buffer, size_t bufferlen);
};


class unsafe_intfd : public impl_intfd {
    public:
        int fd();
};


class safe_intfd : public impl_intfd {
    public:
        ~safe_intfd();
};



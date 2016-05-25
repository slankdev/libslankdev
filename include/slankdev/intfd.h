
#pragma once

#include <stdio.h>
#include <stdint.h>
#include <sys/socket.h> // for struct sockaddr




class intfd {
    private:
        int _fd;
    public:
    
        intfd();
        ~intfd();

        void socket(int domain, int type, int protocol);
        void open(const char* path, int flags);
        void close();
        void bind(const struct sockaddr* sa, size_t len);
        void ioctl(unsigned long l, void* arg);
        void write(const void* buffer, size_t bufferlen);
        size_t read(void* buffer, size_t bufferlen);
};

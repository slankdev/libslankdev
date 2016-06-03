
#pragma once

#include <vector>
#include <string>

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <poll.h>


namespace slankdev {


class pollfd {
    private:
        std::vector<struct ::pollfd>     _pfd;
        std::vector<std::string>       _names;
        ssize_t name_to_index(const std::string& name);

    public:
        pollfd();
        ~pollfd();
        size_t num_ifs();
        void add_if(const std::string& name);
        void del_if(const std::string& name);
        void send(const std::string& name, const void* buf, size_t nbyte);
        size_t recv(const std::string& name, void* buf, size_t nbyte);
        size_t recv_any(std::string& name, void* buf, size_t nbyte);
};




} /* namespace slankdev */

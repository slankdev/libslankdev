
#pragma once

#include <vector>
#include <string>

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <poll.h>

class base {
    private:
        std::vector<struct pollfd>     _pfd;
        std::vector<unsafe_intfd*> _ifs;
        std::vector<std::string>       _names;
        ssize_t name_to_index(const std::string& name);

    public:
        base();
        ~base();
        size_t num_ifs();
        void add_if(const std::string& name);
        void rm_if(const std::string& name);
        void send(const std::string& name, const void* buf, size_t nbyte);
        size_t recv(const std::string& name, void* buf, size_t nbyte);
        size_t recv_any(std::string& name, void* buf, size_t nbyte);
};




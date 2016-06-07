
#include <slankdev/intfd.h>
#include <slankdev/pollfd.h>

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>

#include <arpa/inet.h>
#include <netpacket/packet.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>

namespace slankdev {





pollfd::pollfd() {}
pollfd::~pollfd()
{
    unsafe_intfd fd;
    for (size_t i=0; i<_pfd.size(); i++) {
        fd.fd = _pfd[i].fd;
        fd.close();
    }
}

ssize_t pollfd::name_to_index(const std::string& name) 
{
    for (ssize_t index=0, sz=static_cast<ssize_t>(_names.size()); index<sz; index++) {
        if (_names[index] == name) {
            return index;
        }
    }
    return -1;
}

void pollfd::add_if(const std::string& name)
{
    struct ::pollfd pfd;

    unsafe_intfd fd;
    fd.open_if(name.c_str());

    memset(&pfd, 0, sizeof pfd);
    pfd.fd = fd.fd;
    pfd.events = POLLIN | POLLERR;

    _names.push_back(name);
    _pfd.push_back(pfd);
}

void pollfd::del_if(const std::string& name)
{
    int index = name_to_index(name);

    if (index >= 0) {
        _pfd.erase(_pfd.begin() + index);
        _names.erase(_names.begin() + index);
    }
}

size_t pollfd::num_ifs()
{
    return _pfd.size();
}

void pollfd::send(const std::string& name, const void* buf, size_t nbyte)
{
    int index = name_to_index(name);
    unsafe_intfd fd;
    fd.fd = _pfd[index].fd;

    fd.write(buf, nbyte);   
}

size_t pollfd::recv(const std::string& name, void* buf, size_t nbyte)
{
    int index = name_to_index(name);
    unsafe_intfd fd;
    fd.fd = _pfd[index].fd;

    return fd.read(buf, nbyte);   
}

size_t pollfd::recv_any(std::string& name, void* buf, size_t nbyte)
{
    unsafe_intfd fd;
    poll(_pfd.data(), _pfd.size(), -1);
    for (size_t i=0; i<_pfd.size(); i++) {
        if (_pfd[i].revents & POLLIN) {
            name = _names[i];
            fd.fd = _pfd[i].fd;
            return fd.read(buf, nbyte);
        }
    }
    return 0;
}



} /* namespace slankdev */

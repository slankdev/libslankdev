
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

static unsafe_intfd* open_if(const std::string& name)
{
    unsafe_intfd *fd = new unsafe_intfd();
    fd->socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    struct ifreq ifreq;
    memset(&ifreq, 0, sizeof(ifreq));
    strncpy(ifreq.ifr_name, name.c_str(), sizeof(ifreq.ifr_name)-1);
    fd->ioctl(SIOCGIFINDEX, &ifreq);

    struct sockaddr_ll sa;
    sa.sll_family = PF_PACKET;
    sa.sll_protocol = htonl(ETH_P_ALL);
    sa.sll_ifindex = ifreq.ifr_ifindex;
    fd->bind((struct sockaddr*)&sa, sizeof(sa));

    fd->ioctl(SIOCGIFFLAGS, &ifreq);
    ifreq.ifr_flags = ifreq.ifr_flags | IFF_PROMISC;
    fd->ioctl(SIOCSIFFLAGS, &ifreq);

    return fd;
}


base::base() 
{
}

base::~base()
{
    for (size_t i=0; i<_ifs.size(); i++) {
        delete _ifs[i];
    }
}

ssize_t base::name_to_index(const std::string& name) 
{
    ssize_t index;
    for (index=0; index< static_cast<ssize_t>(_names.size()); index++) {
        if (_names[index] == name) {
            goto success;
        }
    }
    index = -1;
success:
    return index;
}

void base::add_if(const std::string& name)
{
    struct pollfd pfd;

    unsafe_intfd* iface = open_if(name);

    memset(&pfd, 0, sizeof pfd);
    pfd.fd = iface->fd();
    pfd.events = POLLIN | POLLERR;

    _names.push_back(name);
    _pfd.push_back(pfd);
    _ifs.push_back(iface);
}

void base::rm_if(const std::string& name)
{
    int index = name_to_index(name);

    if (index >= 0) {
        _ifs.erase(_ifs.begin() + index);
        _pfd.erase(_pfd.begin() + index);
        _names.erase(_names.begin() + index);
    }
}

size_t base::num_ifs()
{
    return _ifs.size();
}

void base::send(const std::string& name, const void* buf, size_t nbyte)
{
    int index = name_to_index(name);
    _ifs[index]->write(buf, nbyte);   
}

size_t base::recv(const std::string& name, void* buf, size_t nbyte)
{
    int index = name_to_index(name);
    return _ifs[index]->read(buf, nbyte);   
}

size_t base::recv_any(std::string& name, void* buf, size_t nbyte)
{

    poll(_pfd.data(), _pfd.size(), -1);
    for (size_t i=0; i<_pfd.size(); i++) {
        if (_pfd[i].revents & POLLIN) {
            name = _names[i];
            return _ifs[i]->read(buf, nbyte);
        }
    }
    return 0;
}


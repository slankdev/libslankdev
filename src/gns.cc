

#include <slankdev/gns.h>
#include <algorithm>


namespace slankdev {


gns::gns() : running(false) {}
gns::~gns() {}



void gns::add_if(const std::string& name)
{
    pfd.add_if(name);
}
void gns::del_if(const std::string& name)
{
    pfd.del_if(name);
}



void gns::add_callback(gns_callback func, uint8_t priority)
{
    callbacks.push_back(std::pair<gns_callback, uint8_t>(func, priority));
}
void gns::del_callback(gns_callback func)
{
    for (size_t i=0; i<callbacks.size(); i++) {
        if (func == callbacks[i].first) {
            callbacks.erase(callbacks.begin() + i);
            return ;
        }
    }
}



void gns::start()
{
    std::sort(callbacks.begin(), callbacks.end());

    running = true;
    while (running) {
        std::string name;
        uint8_t buf[5000];
        size_t recvlen = pfd.recv_any(name, buf, sizeof buf);

        struct gns_recvinfo info;
        info.interface = &name;
        info.gns = this;
        for (size_t i=0; i<callbacks.size(); i++) {
            callbacks[i].first(&info, buf, recvlen);
        }
    }
}



void gns::stop()
{
    running = false;
}



} /* namespace slankdev */

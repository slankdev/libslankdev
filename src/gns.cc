

#include <slankdev/gns.h>


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


void gns::add_callback(gns_callback func)
{
    callbacks.push_back(func);
}
void gns::del_callback(gns_callback func)
{
    for (size_t i=0; i<callbacks.size(); i++) {
        if (func == callbacks[i]) {
            callbacks.erase(callbacks.begin() + i);
            return ;
        }
    }
}


void gns::start()
{
    running = true;
    while (running) {
        std::string name;
        uint8_t buf[10000];
        size_t recvlen = pfd.recv_any(name, buf, sizeof buf);

        struct gns_recvinfo info;
        info.interface = &name;
        info.gns = this;
        for (size_t i=0; i<callbacks.size(); i++) {
            callbacks[i](&info, buf, recvlen);
        }
    }
}



void gns::stop()
{
    running = false;
}



} /* namespace slankdev */

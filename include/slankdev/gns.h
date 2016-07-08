
#pragma once


#include <slankdev/pollfd.h>
#include <string>
#include <vector>



namespace slankdev {



class gns;


struct gns_recvinfo {
    std::string* interface;
    class gns* gns;
};



typedef void (*gns_callback)(struct gns_recvinfo* info, const void*, size_t);



class gns {
    private:
        slankdev::pollfd pfd;
        std::vector<std::pair<gns_callback, uint8_t> > callbacks;
        bool running;

    public:

        gns();
        ~gns();
        void add_if(const std::string& name);
        void del_if(const std::string& name);
        void add_callback(gns_callback func, uint8_t priority);
        void del_callback(gns_callback func);
        void start();
        void stop();
};



} /* namespace slankdev */

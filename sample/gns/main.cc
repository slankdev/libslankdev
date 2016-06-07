

#include <stdio.h>
#include <slankdev/gns.h>
#include <slankdev.h>
const char* dev = "enp3s0";


using namespace slankdev;



void callback2(struct gns_recvinfo* info, const void* data, size_t nbyte)
{
    printf("callback2\n");
}


void callback(struct gns_recvinfo* info, const void* data, size_t nbyte)
{
    hexdump("callback", data, nbyte);

    // bool isexit = true;
    // if (isexit) {
    //     gns->stop();
    // }
}


int main() 
{
    gns base;
    base.add_if(dev);
    base.add_callback(callback);
    base.add_callback(callback2);
    base.start();
}

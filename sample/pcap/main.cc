
#include <stdio.h>
#include <slankdev.h>
const char* dev = "enp3s0";

using namespace slankdev;

int main() 
{
    safe_intfd fd;
    fd.open_if(dev);

    while (1) {
        uint8_t buf[10000];
        size_t recvlen = fd.read(buf, sizeof buf);
        hexdump("Recieve Packet!!", buf, recvlen);
    }
}

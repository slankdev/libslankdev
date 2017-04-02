
#include <stdio.h>
#include <slankdev/socketfd.h>
#include <slankdev/hexdump.h>
using namespace slankdev;

int main(int argc, char** argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s device \n", argv[0]);
        return -1;
    }

    socketfd fd;
    fd.open_afpacket(argv[1]);

    while (1) {
        uint8_t buf[10000];
        size_t recvlen = fd.read(buf, sizeof buf);
        slankdev::hexdump("Recieve Packet", buf, recvlen);
    }
}

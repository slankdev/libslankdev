
#include <stdio.h>
#include <slankdev.h>
using namespace slankdev;

int main(int argc, char** argv) 
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s device \n", argv[0]);
        return -1;
    }

    socketfd fd;
    fd.open_if(argv[1]);

    while (1) {
        uint8_t buf[10000];
        size_t recvlen = fd.read(buf, sizeof buf);
        hexdump("Recieve Packet", buf, recvlen);
    }
}

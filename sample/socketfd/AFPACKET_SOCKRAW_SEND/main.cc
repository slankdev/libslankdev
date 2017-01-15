
#include <stdlib.h>
#include <slankdev/socketfd.h>
#include <slankdev/util.h>
using namespace slankdev;
#define CONST_DEV true
#define CONST_DEVNAME "enp2s0f0"
#define CONST_CNT  10


uint8_t arp_raw[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x08, 0x06, 0x00, 0x01,
    0x08, 0x00, 0x06, 0x04, 0x00, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
    0x00, 0x00 };


int main_normal(int argc, char** argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s ifname\n", argv[0]);
        return -1;
    }

    socketfd fd;
    fd.open_if(argv[1]);

    hexdump("Sending", arp_raw, sizeof(arp_raw));

    if (argc > 3) {
        for (int i=0; i<atoi(argv[2]); i++)
            fd.write(arp_raw, sizeof(arp_raw));
    } else {
        fd.write(arp_raw, sizeof(arp_raw));
    }
}

int main_const_dev(int argc, char** argv)
{
    socketfd fd;
    fd.open_if(CONST_DEVNAME);

    hexdump("Sending", arp_raw, sizeof(arp_raw));

    if (argc > 2) {
        for (int i=0; i<atoi(argv[1]); i++)
            fd.write(arp_raw, sizeof(arp_raw));
    } else {
        for (int i=0; i<CONST_CNT; i++)
            fd.write(arp_raw, sizeof(arp_raw));
    }

}

int main(int argc, char** argv)
{
    if (CONST_DEV) return main_const_dev(argc, argv);
    else           return main_normal(argc, argv);
}



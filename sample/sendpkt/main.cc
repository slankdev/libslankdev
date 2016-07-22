
#include <stdlib.h>
#include <slankdev.h>
using namespace slankdev;

int main(int argc, char** argv)
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s iface cnt\n", argv[0]);
        return -1;
    }
    socketfd fd;
    fd.open_if(argv[1]);

    hexdump("Sending", arp_raw, sizeof(arp_raw));
    for (int i=0; i<atoi(argv[2]); i++)
        fd.write(arp_raw, sizeof(arp_raw));
}

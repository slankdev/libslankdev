
#include <slankdev.h>
const char* dev = "lo";

int main()
{
    slankdev::safe_intfd fd;
    fd.open_if(dev);

    slankdev::hexdump("out", slankdev::arp_raw, sizeof(slankdev::arp_raw));
    fd.write(slankdev::arp_raw, sizeof(slankdev::arp_raw));
}

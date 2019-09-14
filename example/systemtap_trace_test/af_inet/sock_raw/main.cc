


#include <slankdev.h>
#include <string.h>
#include <stdlib.h>


int main()
{
    slankdev::safe_intfd fd;
    fd.socket(AF_INET, SOCK_RAW, IPPROTO_RAW);

    struct sockaddr_in dst;
    slankdev::string2binary_in4("192.168.222.101", &dst);
    dst.sin_family = AF_INET;
    struct sockaddr_in src;
    slankdev::string2binary_in4("192.168.222.100", &src);

    struct slankdev::ip ip;
    ip.version = 4;
    ip.ihl = sizeof(struct slankdev::ip)/4;
    ip.tos = 0;
    ip.len = htons(sizeof(struct slankdev::ip));
    ip.id = htons(555);
    ip.off = htons(0x4000);
    ip.ttl = 100;
    ip.protocol = IPPROTO_RAW;
    ip.sum = 0;
    ip.src.s_addr = 0;
    ip.dst.s_addr = dst.sin_addr.s_addr;

    fd.sendto(&ip, sizeof(ip), 0, (struct sockaddr*)&dst, sizeof dst);
}

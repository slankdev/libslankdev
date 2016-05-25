

#include <stdio.h>
#include <stdint.h>
#include <slankdev/utils.h>
#include <slankdev/system.h>


int main()
{
    uint8_t buf[100];

    uint64_t now = rdtsc();
    hexdump("test", buf, sizeof buf);
    printf("%lu \n", rdtsc() - now);
}


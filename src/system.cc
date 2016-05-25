
#include <slankdev/system.h>


uint64_t get_rdtsc()
{
    unsigned int eax, edx;
    __asm__ volatile("rdtsc" : "=a"(eax), "=d"(edx));
    return ((uint64_t)edx << 32) | eax;
}



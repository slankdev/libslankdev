
#include <slankdev/system.h>

namespace slankdev {

uint64_t rdtsc()
{
    unsigned int eax, edx;
    __asm__ volatile("rdtsc" : "=a"(eax), "=d"(edx));
    return ((uint64_t)edx << 32) | eax;
}

} /* namespace slankdev */

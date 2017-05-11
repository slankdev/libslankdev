
#pragma once
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sched.h>


namespace slankdev {

class cpuset {
  cpu_set_t cpuset_;

 public:
  cpuset() { CPU_ZERO(&cpuset_); }
  cpuset(uint64_t pref) : cpuset() { set(pref); }
  uint64_t get() const
  {
    uint64_t pref = 0;
    for (size_t i=0; i<64; i++) {
      if (CPU_ISSET(i, &cpuset_)) {
        pref |= 1 << i;
      }
    }
    return pref;
  }
  void set(uint64_t prefix)
  {
    for (size_t i=0; prefix!=0; i++) {
      if (prefix & 0x1) {
        CPU_SET(i, &cpuset_);
      }
      prefix >>= 1;
    }
  }
  void print()
  {
    for (size_t i=0; i<CPU_SETSIZE; i++) {
      if (CPU_ISSET(i, &cpuset_)) {
        printf("+ affinity: %zd \n", i);
      }
    }
  }
  cpu_set_t* get_native() { return &cpuset_; }
  void set_native(cpu_set_t* a) { memcpy(&cpuset_, a, sizeof(cpuset_)); }
};

} /* namespace slankdev */



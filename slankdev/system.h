
/*
 * MIT License
 *
 * Copyright (c) 2017 Susanoo G
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/**
 * @file   slankdev/system.h
 * @brief  system operations
 * @author Hiroki SHIROKURA
 * @date   2017.12.18
 */



#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <slankdev/exception.h>
#include <slankdev/system.h>
#include <slankdev/filefd.h>
#include <vector>


namespace slankdev {

inline std::string uname_get_sysname()
{
  struct utsname val;
  int ret = uname(&val);
  if (ret < 0)
    throw slankdev::exception("uname");
  return val.sysname;
}
inline std::string uname_get_nodename()
{
  struct utsname val;
  int ret = uname(&val);
  if (ret < 0)
    throw slankdev::exception("uname");
  return val.nodename;
}
inline std::string uname_get_release()
{
  struct utsname val;
  int ret = uname(&val);
  if (ret < 0)
    throw slankdev::exception("uname");
  return val.release;
}
inline std::string uname_get_version()
{
  struct utsname val;
  int ret = uname(&val);
  if (ret < 0)
    throw slankdev::exception("uname");
  return val.version;
}
inline std::string uname_get_machine()
{
  struct utsname val;
  int ret = uname(&val);
  if (ret < 0)
    throw slankdev::exception("uname");
  return val.machine;
}



inline uint64_t rdtscp(uint32_t* cpuid)
{
  unsigned int eax, edx;
  __asm__ volatile("rdtscp"
      : "=a"(eax), "=d"(edx)
      :: "%rcx");
  __asm__ volatile(
      "movl %%ecx, %0;"
      :"=r"(*cpuid));
  return ((uint64_t)edx << 32) | eax;
}


inline uint64_t rdtsc()
{
  unsigned int eax, edx;
  __asm__ volatile("rdtsc" : "=a"(eax), "=d"(edx));
  return ((uint64_t)edx << 32) | eax;
}

inline void delay_clk(size_t num_clks)
{
  uint64_t begin = rdtsc();
  while (rdtsc() - begin < num_clks) ;
}

#ifdef __linux__
inline void daemon()
{
  if (::daemon(0, 0) != 0) {
    throw slankdev::exception("daemon");
  }
}

inline size_t n_processors()
{
  return sysconf(_SC_NPROCESSORS_CONF);
}
#endif


#ifdef __linux__
class cpus_state {

  class cpu_state {

    struct cpu_state_raw {
      const size_t core_id;

      long user;
      long nice;
      long system;
      long idel;
      long iowait;
      long irq;
      long softirq;

      cpu_state_raw& operator=(cpu_state_raw& rhs)
      {
        user    = rhs.user   ;
        nice    = rhs.nice   ;
        system  = rhs.system ;
        idel    = rhs.idel   ;
        iowait  = rhs.iowait ;
        irq     = rhs.irq    ;
        softirq = rhs.softirq;
        return *this;
      }

      cpu_state_raw(size_t cid) : core_id(cid) {}
      void update()
      {
        slankdev::filefd file;
        file.fopen("/proc/stat", "r");

        char str[BUFSIZ];
        file.fgets(str, sizeof(str));
        for (size_t i=0; i<core_id; i++) {
          file.fgets(str, sizeof(str));
        }

        file.fgets(str, sizeof(str));
        uint32_t cid;
        sscanf(str, "cpu%u %ld %ld %ld %ld %ld %ld %ld", &cid,
            &this->user, &this->nice, &this->system, &this->idel,
            &this->iowait, &this->irq, &this->softirq);

        if (cid != core_id) {
          printf("cid: %u  (except:%zd)\n", cid, core_id);
          throw slankdev::exception("OKASHII");
        }
      }
    }; /* class cpu_state_raw */

   protected:

    cpu_state_raw curr;
    cpu_state_raw prev;

    long cpu_all_time() const
    {
      long ret = (curr.user - prev.user) +
        (curr.nice - prev.nice) +
        (curr.system - prev.system) +
        (curr.idel - prev.idel) +
        (curr.irq - prev.irq) +
        (curr.softirq - prev.softirq);
      return ret;
    }

    long cpu_used_time() const
    {
      long ret = (curr.user - prev.user) +
        (curr.nice - prev.nice) +
        (curr.system - prev.system) +
        (curr.iowait - prev.iowait) +
        (curr.irq - prev.irq) +
        (curr.softirq - prev.softirq);
      return ret;
    }

   public:

    cpu_state(size_t cid) : curr(cid), prev(cid) {}

    void update()
    {
      prev = curr;
      curr.update();
    }

    double cpu_rate() const
    {
      long all = cpu_all_time();
      long used = cpu_used_time();
      double rate = (double)used/all*100;
      if(rate > 100){
        rate = 100;
      }
      return rate;
    }

  }; /* class cpu_state */

  const size_t np_;
  std::vector<cpu_state> cpus;
 public:

  size_t n_processors() const { return np_; }

  const cpu_state& get_processor(size_t core_id) const
  { return cpus[core_id]; }

  void update()
  {
    const size_t n_ele = cpus.size();
    for (size_t i=0; i<n_ele; i++) {
      cpus[i].update();
    }
  }

  cpus_state() : np_(slankdev::n_processors())
  {
    for (size_t i=0; i<n_processors(); i++) {
      cpus.push_back(cpu_state(i));
    }
  }
};
#endif


} /* namespace slankdev */


#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include <iostream>
#include <thread>
#include <slankdev/exception.h>
#include <slankdev/system.h>
#include <slankdev/cpuset.h>




namespace slankdev {


class thread : public std::thread {
 public:

  thread() : std::thread() {}
  template< class Function, class... Args >
  explicit thread( Function&& f, Args&&... args ) : std::thread(f, args...) {}


  void getaffinity_np(size_t cpulen, cpu_set_t* cpuset)
  {
    int ret = pthread_getaffinity_np(this->native_handle(), cpulen, cpuset);
    if (ret < 0) {
      throw slankdev::exception("pthread_getaffinity_np");
    }
  }

  void setaffinity_np(size_t cpulen , cpu_set_t* cpuset)
  {
    int ret = pthread_setaffinity_np(this->native_handle(), cpulen, cpuset);
    if (ret < 0) {
      throw slankdev::exception("pthread_setaffinity_np");
    }
  }
};


class thread_self : public slankdev::thread {
 public:

  thread_self() {}

  void getaffinity_np(size_t cpulen, cpu_set_t* cpuset)
  {
    pthread_t th = pthread_self();
    int ret = pthread_getaffinity_np(th, cpulen, cpuset);
    if (ret < 0) {
      throw slankdev::exception("pthread_getaffinity_np");
    }
  }

  void setaffinity_np(size_t cpulen , cpu_set_t* cpuset)
  {
    pthread_t th = pthread_self();
    int ret = pthread_setaffinity_np(th, cpulen, cpuset);
    if (ret < 0) {
      throw slankdev::exception("pthread_setaffinity_np");
    }
  }
};



} /* namespace slankdev */

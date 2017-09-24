
#pragma once

#include <string.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <slankdev/intfd.h>
#include <slankdev/socketfd.h>



namespace slankdev {

class tap : public slankdev::safe_intfd {
 public:
  tap(const char* name)
  {
    struct ifreq ifr;
    this->open("/dev/net/tun", O_RDWR);

    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_flags = IFF_TAP;
    strncpy(ifr.ifr_name, name, IFNAMSIZ);
    this->ioctl(TUNSETIFF, (void*)&ifr);

    if (strcmp(name, ifr.ifr_name) != 0)
      throw slankdev::exception("set name is different");

    slankdev::socketfd::linkup(name);
  }
};

} /* namespace slankdev */



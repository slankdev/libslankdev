
#pragma once

#include <pcap.h>
#include <slankdev/exception.h>
#include <slankdev/util.h>
#include <slankdev/string.h>

namespace slankdev {


class pcap {
  pcap_t* handle;
 public:
  pcap() : handle(nullptr) {}
  virtual ~pcap() { close(); }

  int get_selectable_fd()
  {
    int res = pcap_get_selectable_fd(handle);
    if (res < 0) {
      throw slankdev::exception("pcap_get_selectable_fd");
    }
    return res;
  }
  void open_live(const char* ifname)
  {
    if (handle)
      throw slankdev::exception("pcap: exist open discriptor");

    char errbuf[PCAP_ERRBUF_SIZE];
    handle = pcap_open_live(ifname, BUFSIZ, 0, 0, errbuf);
    if (!handle) {
      std::string errstr;
      errstr = slankdev::fs("pcap_open_live: %s", errbuf);
      throw slankdev::exception(errstr.c_str());
    }
  }
  void open_offline(const char* filepath)
  {
    if (handle)
      throw slankdev::exception("pcap: exist open discriptor");

    char errbuf[PCAP_ERRBUF_SIZE];
    handle = pcap_open_offline(filepath, errbuf);
    if (!handle) {
      std::string errstr;
      errstr = slankdev::fs("pcap_open_offline: %s", errbuf);
      throw slankdev::exception(errstr.c_str());
    }
  }
  void close()
  {
    if (handle) {
      pcap_close(handle);
      handle = nullptr;
    }
  }
  void next()
  {
    struct pcap_pkthdr header;
    const u_char *packet = pcap_next(handle, &header);
    if (packet == nullptr) {
      throw slankdev::exception("pcap_next: recvmiss?");
    }
    recv(packet, header.len);
  }
  virtual void recv(const void* ptr, size_t len)
  {
    slankdev::hexdump("Userfunction", ptr, len);
  }
};


} /* namespace slankadev */

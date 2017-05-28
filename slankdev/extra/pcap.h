
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
 * @file   slankdev/pcap.h
 * @brief  pcap wrapper
 * @author Hiroki SHIROKURA
 * @date   2017.4.2
 */


#pragma once

#include <pcap.h>
#include <slankdev/hexdump.h>

namespace slankdev {

inline void cb(uint8_t* user, const struct pcap_pkthdr* h, const uint8_t* byte)
{ slankdev::hexdump(stdout, byte, h->len); }

class pcap {
 protected:
  pcap_t* handle;
 public:
  pcap() : handle(nullptr) {}
  virtual ~pcap() { close(); }

  int get_selectable_fd();
  virtual void open_live(const char* ifname);
  virtual void open_offline(const char* filepath);
  void close();
  void next();
  virtual void recv(const void* ptr, struct pcap_pkthdr* header);
  void setfilter(struct bpf_program *fp);
  void compile(struct bpf_program *fp,
      const char *str, int optimize, bpf_u_int32 netmask);
  void open_dead();
  void loop(int cnt, pcap_handler callback, uint8_t* user);
  int datalink() const;
};


} /* namespace slankadev */




/*
 * Function Implementation
 */

#include <slankdev/exception.h>
#include <slankdev/util.h>
#include <slankdev/string.h>
#include <string>


namespace slankdev {

inline int pcap::datalink() const
{
  return pcap_datalink(handle);
}

inline void pcap::open_dead()
{
  handle = pcap_open_dead(DLT_EN10MB, 65535);
  if (!handle) {
    std::string err;
    err = "pcap_open_dead: ";
    err += pcap_geterr(handle);
    throw slankdev::exception(err.c_str());
  }
}



inline int pcap::get_selectable_fd()
{
  int res = pcap_get_selectable_fd(handle);
  if (res < 0) {
    throw slankdev::exception("pcap_get_selectable_fd");
  }
  return res;
}
inline void pcap::open_live(const char* ifname)
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
inline void pcap::open_offline(const char* filepath)
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
inline void pcap::close()
{
  if (handle) {
    pcap_close(handle);
    handle = nullptr;
  }
}
inline void pcap::next()
{
  struct pcap_pkthdr header;
  const u_char *packet = pcap_next(handle, &header);
  if (packet == nullptr) {
    throw slankdev::exception("pcap_next: recvmiss?");
  }
  recv(packet, &header);
}
inline void pcap::recv(const void* ptr, struct pcap_pkthdr* header)
{
  slankdev::hexdump(stdout, ptr, header->len);
}
inline void pcap::setfilter(struct bpf_program *fp)
{
  int ret = pcap_setfilter(handle, fp);
  if (ret < 0) {
    std::string errstr = pcap_geterr(handle);
    throw slankdev::exception(errstr.c_str());
  }
}
inline void pcap::compile(struct bpf_program *fp,
    const char *str, int optimize, bpf_u_int32 netmask)
{
  int ret = pcap_compile(handle, fp, str, optimize, netmask);
  if (ret < 0) {
    std::string errstr = pcap_geterr(handle);
    throw slankdev::exception(errstr.c_str());
  }
}

inline void pcap::loop(int cnt, pcap_handler callback, uint8_t* user)
{
  pcap_loop(handle, cnt, callback, user);
}

} /* namespace slankadev */







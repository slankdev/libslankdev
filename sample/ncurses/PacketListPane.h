
#pragma once

#include <stdint.h>
#include <stddef.h>
#include <string>

#include "TextPane.h"

class PacketList {
 public:
  PacketList(const void* ptr, size_t len);
  std::string to_str() const;
};

PacketList::PacketList(const void* ptr, size_t len)
{
}
std::string PacketList::to_str() const
{
  return "Packet";
}


class PacketListPane : public TextPane {
  std::vector<PacketList*> packets;
 public:
  PacketListPane(size_t _x, size_t _y, size_t _w, size_t _h)
    : TextPane(_x, _y, _w, _h) {}
  void add_packet(const char* ptr, size_t len)
  {
    packets.push_back(new PacketList(ptr, len));
  }
};


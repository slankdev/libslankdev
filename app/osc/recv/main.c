


#include "packet.h"

int main()
{
  uint8_t data[1000];
  int fd = open_socket("lo");
  size_t rlen = recv_packet(fd, data, sizeof(data));
  hexdump(data, rlen);
}




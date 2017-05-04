
#include <stdio.h>
#include <slankdev/tuntap.h>
#include <slankdev/hexdump.h>


int main()
{
  slankdev::tap tap_in ("tap0");
  slankdev::tap tap_out("tap1");

  while (1) {
    uint8_t buf[10000];
    size_t recvlen = tap_in.read(buf, sizeof(buf));
    printf("Received Packet len=%zd \n", recvlen);
    slankdev::hexdump(stdout, buf, recvlen);
    tap_out.write(buf, recvlen);
  }
}



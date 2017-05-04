

#include <stdio.h>
#include <linux/filter.h>
#include <slankdev/socketfd.h>
#include <slankdev/hexdump.h>

struct sock_filter code[] = {
    BPF_STMT(BPF_LD | BPF_H | BPF_ABS, 12),           // ldh [12]
    BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, 0x806, 0, 1), // jeq #0x06 jt 2 jf 3
    BPF_STMT(BPF_RET | BPF_K, uint32_t(-1)),          // ret #-1
    BPF_STMT(BPF_RET | BPF_K, 0)                      // ret #0
};

struct sock_fprog bpf = {
    .len = sizeof(code)/sizeof(code[0]),
    .filter = code,
};

int main()
{
  slankdev::socketfd sock;
  sock.open_afpacket("lo");
  sock.setsockopt(SOL_SOCKET, SO_ATTACH_FILTER, &bpf, sizeof(bpf));

  while (1) {
    uint8_t buf[10000];
    size_t recvlen = sock.read(buf, sizeof(buf));
    printf("Received Packet len=%zd \n", recvlen);
    slankdev::hexdump(stdout, buf, recvlen);
  }
}



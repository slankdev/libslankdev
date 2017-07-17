
#include "packet.h"
#include "proto.h"
/* #include <arpa/inet.h> */

const uint8_t my_mac[] = {0x70,0x4d,0x7b,0x88,0x3d,0x30};
const uint8_t bcast[]  = {0xff,0xff,0xff,0xff,0xff,0xff};
const uint8_t my_ip[]  = {192,168,100,100};

void Print_ip(const uint8_t* ip) { for (size_t i=0; i<4; i++) printf("%d.", ip[i]); }
void Print_mac(const uint8_t* m) { for (size_t i=0; i<6; i++) printf("%d.", m[i]); }

int main(int argc, char** argv)
{
	if (argc < 3) {
		fprintf(stderr, "Usage: %s ifname target\n", argv[0]);
		return -1;
	}

	uint8_t buf[100];
	memset(buf, 0x0, sizeof(buf));
	int fd = open_socket(argv[1]);

	union u {
		uint32_t u32;
		uint8_t u8[4];
	}U;
	U.u32 = inet_addr(argv[2]);

	uint8_t* ptr = buf;
	struct eth_hdr* eh = (struct eth_hdr*)ptr;
	for (size_t i=0; i<6; i++) {
		eh->src[i] = my_mac[i];
		eh->dst[i] = bcast[i];
	}
	eh->type = ntohs(0x0806);
	ptr += sizeof(struct eth_hdr);

	struct arp_hdr* ah = (struct arp_hdr*)ptr;
  ah->hwtype = htons(0x0001);
  ah->ptype  = htons(0x0800);
  ah->hwlen  = 6;
  ah->plen   = 4;
  ah->operation = htons(1);
	for (size_t i=0; i<6; i++) {
		ah->hwsrc[i] = my_mac[i];
		ah->hwdst[i] = 0x00;
	}
	for (size_t i=0; i<4; i++) {
		ah->psrc[i] = my_ip[i];
		ah->pdst[i] = U.u8[i];
	}
	ptr += sizeof(struct arp_hdr);

	size_t len = ptr - buf;
	send_packet(fd, buf, len);
	printf("ARPING ");
	Print_ip(ah->pdst);
	printf("from ");
	Print_ip(my_ip);
	printf(" %s", argv[1]);
	printf("\n");
	while (1) {
		size_t recvlen = recv_packet(fd, buf, sizeof(buf));
		eh = (struct eth_hdr*)buf;
		ah = (struct arp_hdr*)(eh + 1);
		if (ntohs(eh->type)==0x0806 && ntohs(ah->operation)==2) {
			printf("Unicast reply from ");
			Print_ip(ah->psrc);
			printf("[");
			Print_mac(ah->hwsrc);
			printf("]\n");
			break;
		}
	}
	close(fd);
}




#include "packet.h"

struct ether_hdr {
	uint8_t dst[6];
	uint8_t src[6];
	uint16_t type;
	uint8_t p[];
};

struct ip_hdr {
	uint16_t h;
	uint16_t total_length;
	uint16_t id;
	uint16_t flags;
	uint8_t ttl;
	uint8_t protocol;
	uint16_t checksum;
	uint32_t src_addr;
	uint32_t dst_addr;
	uint8_t p[];
};

void analyze(void* p, size_t l)
{
	printf("recv packet len=%zd\n", l);
	struct ether_hdr* eh = (struct ether_hdr*)(p);
	printf(" dst mac: ");
	for (size_t i=0; i<6; i++) {
		printf("%02x:", eh->dst[i]);
	}
	printf("\n");
	printf(" src mac: ");
	for (size_t i=0; i<6; i++) {
		printf("%02x:", eh->src[i]);
	}
	printf("\n");
	printf(" type: 0x%04x \n", ntohs(eh->type));
	struct ip_hdr* ih = (struct ip_hdr*)(eh->p);
	printf(" src ip: 0x%08x \n", ntohl(ih->src_addr));
	printf(" dst ip: 0x%08x \n", ntohl(ih->dst_addr));
	printf(" protocol: %d\n", ih->protocol);
	hexdump(eh->p, l-sizeof(struct ether_hdr));
	printf("---------\n");
}

int main()
{
  uint8_t data[1000];
  int fd = open_socket("lo");

	while (1) {
		size_t rlen = recv_packet(fd, data, sizeof(data));
		analyze(data, rlen);
	}
}



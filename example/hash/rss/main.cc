
  /*
   * This is Intel-82599-Datasheet's Example
   * tp.dst_addr = (0xa18e6450); // 161.142.100.80 -> 0xa18e6450
   * tp.src_addr = (0x420995bb); // 66.9.149.187   -> 0x420995bb
   * tp.dport    = (0x6e6     ); // 1766           -> 0x6e6
   * tp.sport    = (0xaea     ); // 2794           -> 0xaea
   */






#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <slankdev/mbuf.h>
#include <slankdev/net_header.h>
#include <slankdev/hexdump.h>
#include <slankdev/util.h>

#include <rte_thash.h>



// inline uint32_t IP(uint8_t o1, uint8_t o2, uint8_t o3, uint8_t o4)
// {
//   union {
//     uint8_t  u8[4];
//     uint32_t u32;
//   } U;
//   U.u8[1] = o1;
//   U.u8[2] = o2;
//   U.u8[3] = o3;
//   U.u8[4] = o4;
//   return U.u32;
// }
// inline std::string ip_to_str(uint32_t addr)
// {
//   union {
//     uint8_t  u8[4];
//     uint32_t u32;
//   } U;
//   U.u32 = addr;
//   char str[100];
//   sprintf(str, "%d.%d.%d.%d (%08x)", U.u8[0], U.u8[1], U.u8[2], U.u8[3], addr);
//   return str;
// }
// inline uint32_t bs32(uint32_t b) { return htonl(b); }
// inline uint16_t bs16(uint16_t b) { return htons(b); }





static inline uint8_t* rss_key_def()
{
  static uint8_t r[] = {
    0x6d, 0x5a, 0x56, 0xda, 0x25, 0x5b, 0x0e, 0xc2,
    0x41, 0x67, 0x25, 0x3d, 0x43, 0xa3, 0x8f, 0xb0,
    0xd0, 0xca, 0x2b, 0xcb, 0xae, 0x7b, 0x30, 0xb4,
    0x77, 0xcb, 0x2d, 0xa3, 0x80, 0x30, 0xf2, 0x0c,
    0x6a, 0x42, 0xb7, 0x3b, 0xbe, 0xac, 0x01, 0xfa,
  };
  return r;
};

struct rss_tuple {
  uint32_t src_addr;
  uint32_t dst_addr;
  uint16_t dport;
  uint16_t sport;

  uint32_t hash() const;
};

inline uint32_t slankdev_hash(const struct rss_tuple* data, const void* key)
{
  const uint32_t *input_tuple = reinterpret_cast<const uint32_t*>(data);
  uint32_t input_len = sizeof(rss_tuple)/4;
  const uint32_t *rss_key = reinterpret_cast<const uint32_t*>(key);
	uint32_t ret = 0;

	for (size_t j = 0; j < input_len; j++) {
		for (size_t i = 0; i < 32; i++) {
			if (input_tuple[j] & (1 << (31 - i))) {

        uint32_t a0 = rss_key[j];
        uint32_t a1 = rte_cpu_to_be_32(a0);
        uint32_t a2 = a1 << i;

        uint32_t b0 = rss_key[j+1];
        uint32_t b1 = rte_cpu_to_be_32(b0);
        uint32_t b2 = uint64_t(b1) >> (32-i);
        uint32_t b3 = uint32_t(b2);

				ret ^=  (a2 | b3);

			}
		}
	}
	return ret;
}


uint32_t rss_tuple::hash() const
{
  rss_tuple ss;
  ss.src_addr = src_addr;
  ss.dst_addr = dst_addr;
  ss.sport    = sport   ;
  ss.dport    = dport   ;
  return slankdev_hash(&ss, rss_key_def());
}


int main()
{
  printf("\n\n\n\n");
  uint32_t hash_le;
  struct rte_ipv4_tuple tp;

  uint8_t rss_key[] = {
    0x6d, 0x5a, 0x56, 0xda, 0x25, 0x5b, 0x0e, 0xc2,
    0x41, 0x67, 0x25, 0x3d, 0x43, 0xa3, 0x8f, 0xb0,
    0xd0, 0xca, 0x2b, 0xcb, 0xae, 0x7b, 0x30, 0xb4,
    0x77, 0xcb, 0x2d, 0xa3, 0x80, 0x30, 0xf2, 0x0c,
    0x6a, 0x42, 0xb7, 0x3b, 0xbe, 0xac, 0x01, 0xfa,
  };
  uint8_t* default_rss_key = rss_key;


  tp.src_addr = 0xc0a8000a;
  tp.dst_addr = 0xc0a80001;
  tp.sport    = 0x1234;
  tp.dport    = 0x5678;

  hash_le= rte_softrss(reinterpret_cast<uint32_t*>(&tp), sizeof(tp)/4, default_rss_key);
  printf("LE hash: 0x%08x\n", hash_le);



  rss_tuple rt;
  rt.src_addr = 0xc0a8000a;
  rt.dst_addr = 0xc0a80001;
  rt.sport    = 0x1234;
  rt.dport    = 0x5678;

  hash_le = slankdev_hash(&rt, rss_key_def());
  printf("LE hash: 0x%08x\n", hash_le);

  printf("LE hash: 0x%08x\n", rt.hash());
}






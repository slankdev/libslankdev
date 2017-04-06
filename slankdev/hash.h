
#pragma once

#include <stdint.h>
#include <stddef.h>
#include <slankdev/endian.h>

namespace slankdev {

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



inline uint32_t _rss_hash(const uint32_t* input_tuple,
    size_t input_len, const uint32_t* rss_key)
{
	uint32_t ret = 0;

	for (size_t j = 0; j < input_len; j++) {
		for (size_t i = 0; i < 32; i++) {
			if (input_tuple[j] & (1 << (31 - i))) {

        uint32_t a;
        a = rss_key[j];
        a = htonl(a);
        a = a << i;

        uint32_t b;
        b = rss_key[j+1];
        b = htonl(b);
        b = uint64_t(b) >> (32-i);
        b = uint32_t(b);

				ret ^=  (a | b);

			}
		}
	}
	return ret;
}


struct rss_tuple {
  uint32_t src_addr;
  uint32_t dst_addr;
  uint16_t dport;
  uint16_t sport;

  uint32_t hash(const void* key) const
  {

    rss_tuple ss;
    ss.src_addr = src_addr;
    ss.dst_addr = dst_addr;
    ss.sport    = sport   ;
    ss.dport    = dport   ;

    const uint32_t *input_tuple = reinterpret_cast<const uint32_t*>(&ss);
    uint32_t input_len = sizeof(rss_tuple)/4;
    const uint32_t *rss_key = reinterpret_cast<const uint32_t*>(key);

    return _rss_hash(
      input_tuple,
      input_len,
      rss_key
    );

  }
};



} /* namespace slankdev */




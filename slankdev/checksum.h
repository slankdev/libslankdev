
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
 * @file   slankdev/checksum.h
 * @brief  checksum function
 * @author Hiroki SHIROKURA
 * @date   2017.4.2
 */



#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>


namespace slankdev {


#if 0
/* Thanks @herumi */
static uint16_t read_as_little_endian(const void* data)
{
  const uint8_t* p = reinterpret_cast<const uint8_t*>(data);
  return uint16_t(p[0]) | (uint16_t(p[1]) << 8);
}

inline uint16_t checksum(const void* data, size_t len)
{
  uint32_t sum = 0;
  const uint8_t* data_pointer = reinterpret_cast<const uint8_t*>(data);

  for (; len > 1; len-=2, data_pointer+=2) {
    uint16_t tmp = read_as_little_endian(data_pointer);
    sum += tmp;
  }

  if (len == 1) {
    fprintf(stderr, "pgen::checksum: byte length isn't even num\n");
    uint16_t tmp = read_as_little_endian(data_pointer);
    sum += tmp;
  }

  uint16_t overflowd = sum >> 16;
  sum = sum & 0x0000ffff;
  sum = sum + overflowd;

  return ~sum;
}
#else
inline uint32_t
__raw_cksum(const void *buf, size_t len, uint32_t sum)
{
	uintptr_t ptr = (uintptr_t)buf;
	typedef uint16_t __attribute__((__may_alias__)) u16_p;
	const u16_p *u16 = (const u16_p *)ptr;

	while (len >= (sizeof(*u16) * 4)) {
		sum += u16[0];
		sum += u16[1];
		sum += u16[2];
		sum += u16[3];
		len -= sizeof(*u16) * 4;
		u16 += 4;
	}
	while (len >= sizeof(*u16)) {
		sum += *u16;
		len -= sizeof(*u16);
		u16 += 1;
	}
	if (len == 1) sum += *((const uint8_t *)u16);
	return sum;
}
inline uint16_t __raw_cksum_reduce(uint32_t sum)
{
	sum = ((sum & 0xffff0000) >> 16) + (sum & 0xffff);
	sum = ((sum & 0xffff0000) >> 16) + (sum & 0xffff);
	return (uint16_t)sum;
}

inline uint16_t
checksum(const void *buf, size_t len)
{
	uint32_t sum = __raw_cksum(buf, len, 0);
	return __raw_cksum_reduce(sum);
}
#endif

inline uint16_t
ipv4_checksum(const void* ih_ptr)
{
  const ip* ih = reinterpret_cast<const ip*>(ih_ptr);
	uint16_t cksum;
	cksum = checksum(ih, sizeof(ip));
	return (cksum == 0xffff) ? cksum : ~cksum;
}

static inline uint16_t
ipv4_phdr_cksum(const ip *ipv4_hdr)
{
	struct ipv4_psd_header {
		uint32_t src_addr; /* IP address of source host. */
		uint32_t dst_addr; /* IP address of destination host. */
		uint8_t  zero;     /* zero. */
		uint8_t  proto;    /* L4 protocol type. */
		uint16_t len;      /* L4 length. */
	} psd_hdr;

	psd_hdr.src_addr = ipv4_hdr->src;
	psd_hdr.dst_addr = ipv4_hdr->dst;
	psd_hdr.zero = 0;
	psd_hdr.proto = ipv4_hdr->proto;
  psd_hdr.len = htons(
    (uint16_t)(ntohs(ipv4_hdr->tot_len) - sizeof(ip)));
	return checksum(&psd_hdr, sizeof(psd_hdr));
}

inline uint16_t
ipv4_tcpudp_checksum(const void* ih_ptr, const void* l4_hdr)
{
  const ip* ih = reinterpret_cast<const ip*>(ih_ptr);
	uint32_t l4_len = ntohs(ih->tot_len) - sizeof(ip);

	uint32_t cksum = checksum(l4_hdr, l4_len);
	cksum += ipv4_phdr_cksum(ih);
	cksum = ((cksum & 0xffff0000) >> 16) + (cksum & 0xffff);
	cksum = (~cksum) & 0xffff;
	if (cksum == 0)
		cksum = 0xffff;
	return cksum;
}


} /* namespace slankdev */




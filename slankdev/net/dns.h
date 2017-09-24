
#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <slankdev/hexdump.h>

namespace slankdev {

static inline bool is_ascii_char(char c)
{
  bool b0 = ('a' <= c) && (c <= 'z');
  bool b1 = ('A' <= c) && (c <= 'Z');
  bool b2 = ('0' <= c) && (c <= '9');
  bool ret = b0 || b1 || b2;
  return ret;
}

inline ssize_t domain_reader(const void* domain, size_t len,
        char* str, size_t str_len)
{
  if (str_len < len) {
      return -1;
  }

  const uint8_t* ptr = (const uint8_t*)domain;
  memcpy(str, ptr+1, len-1);
  for (size_t i=0; i<len-2; i++) {
      if (!is_ascii_char(str[i])) str[i] = '.';
    }

  const uint8_t* const ptr_head = (const uint8_t*)domain;
  ptr = (const uint8_t*)domain;
  for (size_t i=0; i<len; i++, ptr++) {
      if (*ptr==0) {
            if (size_t(ptr - ptr_head) > len) return -1;
            return ptr - ptr_head;
          }
      uint8_t cnt = *ptr;
      ptr += cnt;
    }
  return -1;
}

} /* namespace slankdev */


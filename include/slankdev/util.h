

#pragma once 

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <netinet/in.h>

namespace slankdev {


uint16_t checksum(const void* data, size_t len);

void hexdump(const char* title, const void* buffer, size_t bufferlen);
void bindump(const char* title, const void* buffer, size_t bufferlen);

void string2binary_in4(const char* src, struct ::sockaddr_in* dst);
void binary2string_in4(const struct sockaddr_in* src, char* dst, size_t dstlen);



} /* namespace slankdev */

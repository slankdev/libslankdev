

#pragma once 

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

namespace slankdev {


uint16_t checksum(const void* data, size_t len);

void hexdump(const char* title, const void* buffer, size_t bufferlen);
void bindump(const char* title, const void* buffer, size_t bufferlen);

uint16_t htons(uint16_t n);
uint16_t ntohs(uint16_t n);


void clear_screen();



} /* namespace slankdev */

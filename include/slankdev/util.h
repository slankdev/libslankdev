

#pragma once 

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

namespace slankdev {


uint16_t checksum(const void* data, size_t len);

void hexdump(const char* title, const void* buffer, size_t bufferlen);
void bindump(const char* title, const void* buffer, size_t bufferlen);



void clear_screen();



} /* namespace slankdev */

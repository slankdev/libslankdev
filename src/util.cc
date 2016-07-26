

#include <slankdev/util.h>
#include <arpa/inet.h>
#include <stdlib.h>

namespace slankdev {


/* Thanks @herumi */
static uint16_t read_as_little_endian(const void* data)
{
    const uint8_t* p = reinterpret_cast<const uint8_t*>(data);  
    return uint16_t(p[0]) | (uint16_t(p[1]) << 8);
}

/* Thanks @herumi, Isn't used yet */
static uint16_t read_as_big_endian(const void* data)
{
    const uint8_t* p = reinterpret_cast<const uint8_t*>(data);  
    return uint16_t(p[1]) | (uint16_t(p[0]) << 8);
}




uint16_t checksum(const void* data, size_t len)
{
    uint32_t sum;
    const uint8_t* data_pointer = reinterpret_cast<const uint8_t*>(data);
    
    for (; len > 1; len-=2, data_pointer+=2) {
        uint16_t tmp = read_as_little_endian(data_pointer);
        sum += tmp;
    }
    
    if (len == 1) {
        fprintf(stderr, "pgen::checksum: byte length isn't even num\n");
        uint8_t tmp = *data_pointer;
        sum += tmp;
    }
    
    uint16_t overflowd = sum >> 16;
    sum = sum & 0x0000ffff;
    sum = sum + overflowd;
    
    return ~sum;
}



void hexdump(const char* title, const void *buffer, size_t bufferlen)
{
    printf("%s at [%p] length=%zd \n", title, buffer, bufferlen);

    const uint8_t *data = reinterpret_cast<const uint8_t*>(buffer);
    size_t row = 0;
    while (bufferlen > 0) {
        printf("%04zx:   ", row);
        
        size_t n;
        if (bufferlen < 16) n = bufferlen;
        else                n = 16;

        for (size_t i = 0; i < n; i++) {
            if (i == 8) printf(" ");
            printf(" %02x", data[i]);
        }
        for (size_t i = n; i < 16; i++) {
            printf("   ");
        }
        printf("   ");
        for (size_t i = 0; i < n; i++) {
            if (i == 8) printf("  ");
            uint8_t c = data[i];
            if (!(0x20 <= c && c <= 0x7e)) c = '.';
            printf("%c", c);
        }
        printf("\n");
        bufferlen -= n;
        data += n;
    }
}


void bindump(const char* title, const void* buffer, size_t bufferlen) 
{
    printf("%s at [%p] length=%zd \n", title, buffer, bufferlen);

    const uint8_t* p0 = reinterpret_cast<const uint8_t*>(buffer);
    char buf[] = "00000000 ";
    for (size_t i=0; i<bufferlen; i++) {
        uint8_t c = p0[i];
        for (size_t j = 0; j < 8; j++) {
            buf[j] = '0' + (c & 1);
            c >>= 1;
        }
        printf("%s", buf);
        if ((i+1) % 8 == 0) printf("\n");
    }
} 





void string2binary_in4(const char* src, struct ::sockaddr_in* dst)
{
    int res = inet_pton(AF_INET, src, &dst->sin_addr);
    if (res != 1) {
        perror("inet_pton");
        exit(-1);
    }
}




void binary2string_in4(const struct sockaddr_in* src, char* dst, size_t dstlen)
{
    const char* res = inet_ntop(AF_INET, &src->sin_addr, dst, dstlen);
    if (res == NULL) {
        perror("inet_ntop");
        exit(-1);
    }
}





} /* namespace slankdev */
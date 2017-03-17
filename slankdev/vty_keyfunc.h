
#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <string>
#include <vector>

#include <slankdev/exception.h>



namespace slankdev {



class shell;

class key_func {
public:
    uint8_t code[256];
    const size_t  len;
    key_func(const void* d, size_t l) : len(l)
    {
        if (sizeof(code) < l) throw slankdev::exception("length is too big");
        memcpy(code, d, l);
    }
    virtual ~key_func() {}
    virtual void function(shell*) = 0;
    virtual bool match(const void* d, size_t l)
    {
        return (len == l) && (memcmp(code, d, l) == 0);
    }
};


} /* namespace slankdev */





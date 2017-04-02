

#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <vector>

#define UNUSED(x) (void)(x)

namespace slankdev {


template <class T>
static inline void vec_delete_all_ptr_elements(std::vector<T*>& vec)
{
    while (vec.size() > 0) {
        T* tmp = vec.back();
        vec.pop_back();
        delete tmp;
    }
}

template <class... ARGS>
inline void fdprintf(int fd, const char* fmt, ARGS... args)
{
    FILE* fp = fdopen(fd, "w");
    ::fprintf(fp, fmt, args...);
    fflush(fp);
}


inline void clear_screen()
{
    printf("\033[2J\n");
}



} /* namespace slankdev */

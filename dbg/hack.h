
#ifndef _SLANKDEV_DBG_
#define _SLANKDEV_DBG_

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <malloc.h>
#include <stdbool.h>

#define hack_printf(...) \
    do { \
      char str[100]; \
      sprintf(str, __VA_ARGS__); \
      write(1, str, strlen(str)+1); \
    } while (0)

extern size_t num_malloc_called  ;
extern size_t num_realloc_called ;
extern size_t num_calloc_called  ;
extern size_t num_free_called    ;
extern size_t num_memalign_called;
extern size_t sum_malloced_memory;
extern size_t sum_freed_memory   ;
extern size_t cur_alloced_memory ;
extern bool   malloc_detail;

void* (*myfn_calloc)(size_t nmemb, size_t size);
void* (*myfn_malloc)(size_t size);
void  (*myfn_free)(void *ptr);
void* (*myfn_realloc)(void *ptr, size_t size);
void* (*myfn_memalign)(size_t blocksize, size_t bytes);

void init();
void set_all_sighandlers();
void print_backtrace();

#endif /* _SLANKDEV_DBG_ */


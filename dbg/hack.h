
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

#define hack_printf(...) \
    do { \
      char str[100]; \
      sprintf(str, __VA_ARGS__); \
      write(1, str, strlen(str)+1); \
    } while (0)

extern char tmpbuff[1024];
extern unsigned long tmppos;
extern unsigned long tmpallocs;

extern size_t num_malloc_called  ;
extern size_t num_realloc_called ;
extern size_t num_calloc_called  ;
extern size_t num_free_called    ;
extern size_t num_memalign_called;
extern size_t sum_malloced_memory;
extern size_t sum_freed_memory   ;
extern size_t cur_alloced_memory ;

void *memset(void*,int,size_t);
void *memmove(void *to, const void *from, size_t size);

void* (*myfn_calloc)(size_t nmemb, size_t size);
void* (*myfn_malloc)(size_t size);
void  (*myfn_free)(void *ptr);
void* (*myfn_realloc)(void *ptr, size_t size);
void* (*myfn_memalign)(size_t blocksize, size_t bytes);

#endif /* _SLANKDEV_DBG_ */


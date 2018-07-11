
#include "hack.h"

size_t num_malloc_called   = 0;
size_t num_realloc_called  = 0;
size_t num_calloc_called   = 0;
size_t num_free_called     = 0;
size_t num_memalign_called = 0;
size_t sum_malloced_memory = 0;
size_t sum_freed_memory    = 0;
size_t cur_alloced_memory  = 0;

void cnstr() __attribute__((constructor));
void cnstr() { }

void dstrctr() __attribute__((destructor));
void dstrctr() { }

void init()
{
  set_all_sighandlers();

  myfn_malloc     = dlsym(RTLD_NEXT, "malloc");
  myfn_free       = dlsym(RTLD_NEXT, "free");
  myfn_calloc     = dlsym(RTLD_NEXT, "calloc");
  myfn_realloc    = dlsym(RTLD_NEXT, "realloc");
  myfn_memalign   = dlsym(RTLD_NEXT, "memalign");

  if ( !myfn_malloc
      || !myfn_free
      || !myfn_calloc
      || !myfn_realloc
      || !myfn_memalign) {
    fprintf(stderr, "Error in `dlsym`: %s\n", dlerror());
    exit(1);
  }
}


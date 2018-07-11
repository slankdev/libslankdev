
#include "hack.h"

char tmpbuff[1024];
unsigned long tmppos = 0;
unsigned long tmpallocs = 0;

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


void dump_counter(int signum) {
  printf("\n\n");
  printf("###############################\n");
  printf("Called Signal Handler: dump counter\n");
  printf("num_malloc_called  : %zd \n", num_malloc_called  );
  printf("num_realloc_called : %zd \n", num_realloc_called );
  printf("num_calloc_called  : %zd \n", num_calloc_called  );
  printf("num_free_called    : %zd \n", num_free_called    );
  printf("num_memalign_called: %zd \n", num_memalign_called);
  printf("\n");
  printf("sum_malloced_memory: %zd \n", sum_malloced_memory);
  printf("sum_freed_memory: %zd \n"   , sum_freed_memory);
  printf("current_allocated_memory: %zd\n", cur_alloced_memory);
  printf("###############################\n");
  printf("\n");
}

void reset_counter(int signum) {
  printf("\n\n");
  printf("###############################\n");
  printf("Called Signal Handler: reset counter\n");
  printf("###############################\n");
  printf("\n");

  num_malloc_called   = 0;
  num_realloc_called  = 0;
  num_calloc_called   = 0;
  num_free_called     = 0;
  num_memalign_called = 0;

  sum_malloced_memory = 0;
  sum_freed_memory    = 0;
  cur_alloced_memory  = 0;

  dump_counter(0);
}

void init()
{
  if (signal(SIGUSR1, dump_counter) == SIG_ERR) {
    fprintf(stderr, "signal: can't set sighandler\n");
    exit(1);
  }
  if (signal(SIGUSR2, reset_counter) == SIG_ERR) {
    fprintf(stderr, "signal: can't set sighandler\n");
    exit(1);
  }

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


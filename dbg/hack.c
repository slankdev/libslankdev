
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <malloc.h>

char tmpbuff[1024];
unsigned long tmppos = 0;
unsigned long tmpallocs = 0;

void *memset(void*,int,size_t);
void *memmove(void *to, const void *from, size_t size);

static void * (*myfn_calloc)(size_t nmemb, size_t size);
static void * (*myfn_malloc)(size_t size);
static void   (*myfn_free)(void *ptr);
static void * (*myfn_realloc)(void *ptr, size_t size);
static void * (*myfn_memalign)(size_t blocksize, size_t bytes);

static size_t num_malloc_called   = 0;
static size_t num_realloc_called  = 0;
static size_t num_calloc_called   = 0;
static size_t num_free_called     = 0;
static size_t num_memalign_called = 0;
static size_t sum_malloced_memory = 0;
static size_t sum_freed_memory    = 0;
static size_t cur_alloced_memory  = 0;

static void cnstr() __attribute__((constructor));
static void cnstr() { }

static void dstrctr() __attribute__((destructor));
static void dstrctr() { }

#define hack_printf(...) \
    do { \
      char str[100]; \
      sprintf(str, __VA_ARGS__); \
      write(1, str, strlen(str)+1); \
    } while (0)

static void dump_counter(int signum) {
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

static void reset_counter(int signum) {
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

static void init()
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

void *malloc(size_t size)
{
  static int initializing = 0;
  if (myfn_malloc == NULL) {
    if (!initializing) {
      initializing = 1;
      init();
      initializing = 0;

      fprintf(stdout, "jcheck: allocated %lu bytes "
          "of temp memory in %lu chunks during initialization\n",
          tmppos, tmpallocs);
    } else {
      if (tmppos + size < sizeof(tmpbuff)) {
        void *retptr = tmpbuff + tmppos;
        tmppos += size;
        ++tmpallocs;
        return retptr;
      } else {
        fprintf(stdout, "jcheck: too much memory requested "
            "during initialisation - increase tmpbuff size\n");
        exit(1);
      }
    }
  }

  num_malloc_called++;
  hack_printf("%s\n", __func__);
  void *ptr = myfn_malloc(size);
  sum_malloced_memory += malloc_usable_size(ptr);
  return ptr;
}

void free(void *ptr)
{
  if (ptr >= (void*) tmpbuff && ptr <= (void*)(tmpbuff + tmppos)) {
    fprintf(stdout, "freeing temp memory\n");
  } else {
    num_free_called++;
    hack_printf("%s\n", __func__);
    sum_freed_memory += malloc_usable_size(ptr);
    myfn_free(ptr);
  }
}

void *realloc(void *ptr, size_t size)
{
  if (myfn_malloc == NULL) {
    hack_printf("realloc: can't use original malloc\n");
    void *nptr = malloc(size);
    if (nptr && ptr) {
      memmove(nptr, ptr, size);
      free(ptr);
    }
    return nptr;
  }

  num_realloc_called++;
  hack_printf("%s\n", __func__);
  void *nptr = myfn_realloc(ptr, size);
  return nptr;
}

void *calloc(size_t nmemb, size_t size)
{
  if (myfn_malloc == NULL) {
    hack_printf("calloc: can't use original malloc\n");
    void *ptr = malloc(nmemb*size);
    if (ptr)
      memset(ptr, 0, nmemb*size);
    return ptr;
  }

  num_calloc_called++;
  hack_printf("%s\n", __func__);
  void *ptr = myfn_calloc(nmemb, size);
  return ptr;
}

void *memalign(size_t blocksize, size_t bytes)
{
  num_memalign_called++;
  hack_printf("%s\n", __func__);
  void *ptr = myfn_memalign(blocksize, bytes);
  return ptr;
}


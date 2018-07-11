
#include "hack.h"
#include <string.h>

char tmpbuff[1024];
unsigned long tmppos = 0;
unsigned long tmpallocs = 0;
bool malloc_detail = false;
struct allocate_records records;

void *memset(void*,int,size_t);
void *memmove(void *to, const void *from, size_t size);

static void add_ptr_to_record(void* ptr)
{
  for (size_t i=0; i<RECORD_MAX; i++) {
    if (records.ptrs[i] == NULL) {
      records.ptrs[i] = ptr;
      return ;
    }
  }
  hack_printf("there are no slot!!\n");
  exit(1);
}

static void del_ptr_from_record(void* ptr)
{
  for (size_t i=0; i<RECORD_MAX; i++) {
    if (records.ptrs[i] == ptr) {
      records.ptrs[i] = NULL;
      return ;
    }
  }
  hack_printf("recored was not found!!\n");
  exit(1);
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
  void *ptr = myfn_malloc(size);
  sum_malloced_memory += malloc_usable_size(ptr);

  hack_printf("%-10s:%p: size=%zd(%zd)\n",
      __func__, ptr, malloc_usable_size(ptr), size);

  add_ptr_to_record(ptr);

  if (malloc_detail) {
    print_backtrace(1);
  }
  return ptr;
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
  void *ptr = myfn_calloc(nmemb, size);

  add_ptr_to_record(ptr);

  hack_printf("%-10s:%p: size=%zd(%zd) \n", __func__,
      ptr, malloc_usable_size(ptr), size);

  if (malloc_detail) {
    print_backtrace(1);
  }
  return ptr;
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

  const size_t old_act_size = malloc_usable_size(ptr);
  sum_malloced_memory -= old_act_size;
  num_realloc_called++;
  void *nptr = myfn_realloc(ptr, size);

  sum_malloced_memory += malloc_usable_size(nptr);

  hack_printf("%-10s:%p: size=%zd->%zd(%zd) oldptr=%p\n", __func__,
      nptr, old_act_size, malloc_usable_size(nptr), size, ptr);

  if (nptr != ptr) {
    del_ptr_from_record(ptr);
    add_ptr_to_record(nptr);
  }

  if (malloc_detail) {
    print_backtrace(1);
  }
  return nptr;
}

void free(void *ptr)
{
  if (ptr >= (void*) tmpbuff && ptr <= (void*)(tmpbuff + tmppos)) {
    fprintf(stdout, "freeing temp memory\n");
    return;
  }

  num_free_called++;
  sum_freed_memory += malloc_usable_size(ptr);

  hack_printf("%-10s:%p: size=%zd\n", __func__,
      ptr, malloc_usable_size(ptr));

  del_ptr_from_record(ptr);
  if (malloc_detail) {
    print_backtrace(1);
  }
  myfn_free(ptr);
}

void *memalign(size_t blocksize, size_t bytes)
{
  num_memalign_called++;
  hack_printf("%-10s\n", __func__);
  void *ptr = myfn_memalign(blocksize, bytes);
  return ptr;
}


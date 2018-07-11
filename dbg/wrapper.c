
#include "hack.h"

void init();

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



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define  __USE_GNU
#include <dlfcn.h>
#include <execinfo.h>
#define UNW_LOCAL_ONLY
#include <libunwind.h>

void print_backtrace(int depth)
{
  unw_cursor_t cursor;
  unw_context_t context;
  unw_getcontext(&context);
  unw_init_local(&cursor, &context);

  size_t count = 0;
  do {
    unw_word_t  offset, pc;
    unw_get_reg(&cursor, UNW_REG_IP, &pc);
    char fname[64];
    memset(fname, 0x0, sizeof(fname));
    unw_get_proc_name(&cursor, fname, sizeof(fname), &offset);

    Dl_info info;
    dladdr((void*)pc, &info);

    if (count > 1) {
      fprintf(stderr, "    #%zd %s(%s+0x%lx) [%p]\n",
          count-2, info.dli_fname, fname, offset, (void*)pc);
    }
    count++;
  } while (unw_step(&cursor) > 0 && count < depth+2);
}

void print_backtrace_full()
{
  print_backtrace(1000);
}


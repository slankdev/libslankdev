
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

  unw_step(&cursor); /* skip frame of this function */
  unw_step(&cursor); /* skip frame of malloc */

  size_t count = 0;
  do {
    char fname[64] = {0};
    unw_word_t  offset, pc;
    unw_get_reg(&cursor, UNW_REG_IP, &pc);
    unw_get_proc_name(&cursor, fname, sizeof(fname), &offset);

    fprintf(stderr, "  #%zd <%s+0x%lx>\n", count, fname, offset);
    count ++;
  } while (unw_step(&cursor) > 0 && count < depth);
}

#if 0
void get_backtrace(struct bt_ctx* array, size_t size)
{
}
#endif

void print_backtrace_full()
{
  print_backtrace(1000);
}


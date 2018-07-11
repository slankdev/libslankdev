
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define  __USE_GNU
#include <dlfcn.h>
#include <execinfo.h>
#define UNW_LOCAL_ONLY
#include <libunwind.h>
#include "hack.h"

void print_backtrace(struct backtrace_ctx* ctx, int depth)
{
  assert(1000 > ctx->size);
  assert(ctx->size >= depth);
  for (size_t i=0; i<depth; i++) {
    fprintf(stderr, "  #%zd <%s+0x%lx>\n", i,
        ctx->frames[i].fname, ctx->frames[i].offset);
  }
}

void get_backtrace(struct backtrace_ctx* ctx)
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

    assert(1000 > count);
    strcpy(ctx->frames[count].fname, fname);
    ctx->frames[count].offset = offset;
    count ++;
  } while (unw_step(&cursor) > 0);
  ctx->size = count;
}


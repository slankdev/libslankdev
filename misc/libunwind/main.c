
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define  __USE_GNU
#include <dlfcn.h>
#include <execinfo.h>

#define UNW_LOCAL_ONLY
#include <libunwind.h>


/* void print_backtrace(size_t depth) */
/* { */
/*   for (size_t i=0; i<depth; i++) { */
/*     Dl_info info; */
/*     dladdr(__builtin_return_address(3), &info); */
/*     fprintf(stderr, "%s : __builtin_return_address => %p\n", __func__, __builtin_return_address(0)); */
/*     fprintf(stderr, "%s : Dl_info.dli_fname => %s\n", __func__, info.dli_fname); */
/*     fprintf(stderr, "%s : Dl_info.dli_fbase => %p\n", __func__, info.dli_fbase); */
/*     fprintf(stderr, "%s : Dl_info.dli_sname => %s\n", __func__, info.dli_sname); */
/*     fprintf(stderr, "%s : Dl_info.dli_saddr => %p\n", __func__, info.dli_saddr); */
/*     printf("-----------\n"); */
/*   } */
/* } */

void print_backtrace(int depth)
{
   unw_cursor_t cursor;
    unw_context_t context;

    unw_getcontext(&context);
    unw_init_local(&cursor, &context);

    size_t count = 0;

    do {
        unw_word_t  offset, pc;
        char        fname[64];

        unw_get_reg(&cursor, UNW_REG_IP, &pc);

        fname[0] = '\0';
        (void) unw_get_proc_name(&cursor, fname, sizeof(fname), &offset);

        Dl_info info;
        dladdr((void*)pc, &info);

        fprintf(stderr, "%s : backtrace [%d] %s(%s+0x%lx) [%p]\n",
                __func__,
                count,
                info.dli_fname,
                fname,
                offset,
                (void*)pc
               );

        count++;

    } while (unw_step(&cursor) > 0);

}

void slankdev() { printf("slankdev\n"); }
void func3()
{
  slankdev();
  /* print_backtrace(3); */
}
void func2() { func3(); }
void func1() { func2(); }

int main(int argc, char** argv)
{
  func1();
}


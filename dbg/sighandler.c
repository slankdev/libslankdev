
#include "hack.h"

static void dump_counter(int signum);
static void reset_counter(int signum);
static void switch_detail(int signum);


static void dump_counter(int signum)
{
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

static void reset_counter(int signum)
{
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

static void switch_detail(int signum)
{
  malloc_detail = !malloc_detail;
  printf("switch_detail malloc_detail:%s\n",
      malloc_detail?"false->true":"true->false");
}

void set_all_sighandlers()
{
  /*
   *  1) SIGHUP        2) SIGINT        3) SIGQUIT
   *  4) SIGILL        5) SIGTRAP       6) SIGABRT
   *  7) SIGBUS        8) SIGFPE        9) SIGKILL
   * 10) SIGUSR1      11) SIGSEGV      12) SIGUSR2
   * 13) SIGPIPE      14) SIGALRM      15) SIGTERM
   * 16) SIGSTKFLT    17) SIGCHLD      18) SIGCONT
   * 19) SIGSTOP      20) SIGTSTP      21) SIGTTIN
   * 22) SIGTTOU      23) SIGURG       24) SIGXCPU
   * 25) SIGXFSZ      26) SIGVTALRM    27) SIGPROF
   * 28) SIGWINCH     29) SIGIO        30) SIGPWR
   * 31) SIGSYS       34) SIGRTMIN     35) SIGRTMIN+1
   * 36) SIGRTMIN+2   37) SIGRTMIN+3   38) SIGRTMIN+4
   * 39) SIGRTMIN+5   40) SIGRTMIN+6   41) SIGRTMIN+7
   * 42) SIGRTMIN+8   43) SIGRTMIN+9   44) SIGRTMIN+10
   * 45) SIGRTMIN+11  46) SIGRTMIN+12  47) SIGRTMIN+13
   * 48) SIGRTMIN+14  49) SIGRTMIN+15  50) SIGRTMAX-14
   * 51) SIGRTMAX-13  52) SIGRTMAX-12  53) SIGRTMAX-11
   * 54) SIGRTMAX-10  55) SIGRTMAX-9   56) SIGRTMAX-8
   * 57) SIGRTMAX-7   58) SIGRTMAX-6   59) SIGRTMAX-5
   * 60) SIGRTMAX-4   61) SIGRTMAX-3   62) SIGRTMAX-2
   * 63) SIGRTMAX-1   64) SIGRTMAX
   */
  if (signal(SIGUSR1, dump_counter) == SIG_ERR) {
    fprintf(stderr, "signal: can't set sighandler\n");
    exit(1);
  }
  if (signal(SIGUSR2, reset_counter) == SIG_ERR) {
    fprintf(stderr, "signal: can't set sighandler\n");
    exit(1);
  }
  if (signal(SIGTERM, switch_detail) == SIG_ERR) {
    fprintf(stderr, "signal: can't set sighandler\n");
    exit(1);
  }
}


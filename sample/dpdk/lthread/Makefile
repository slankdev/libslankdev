
LTHREAD = lthread
SSN := /home/slank/git/susanow
include $(SSN)/mk/vars.mk
CXXFLAGS += -Wno-format-security -g -O0
CFLAGS += -I./$(LTHREAD) -I./$(LTHREAD)/arch/x86 -g -O0

CSRCS = \
	$(LTHREAD)/lthread.c       \
	$(LTHREAD)/lthread_sched.c \
	$(LTHREAD)/lthread_cond.c  \
	$(LTHREAD)/lthread_tls.c   \
	$(LTHREAD)/lthread_mutex.c \
	$(LTHREAD)/lthread_diag.c  \
	$(LTHREAD)/arch/x86/ctx.c
COBJS = $(CSRCS:.c=.o)
CXXSRCS = main.cc
CXXOBJS = $(CXXSRCS:.cc=.o)
OBJS = $(CXXOBJS) $(COBJS)
TARGET = a.out
include $(SSN)/mk/rules.mk




all: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

clean:
	rm -rf $(OBJS) a.out

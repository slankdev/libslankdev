
LIB = /home/slank/git/libslankdev
include $(LIB)/mk/dpdk.mk

CXXFLAGS += -I$(LIB) -std=c++11 $(DPDK_CFLAGS) -Wall -Wextra
LDFLAGS  += $(DPDK_LDFLAGS)

all:
	$(CXX) $(CXXFLAGS) main.cc $(LDFLAGS)
	./a.out


LIB = /home/slank/git/slankdev/libslankdev
include $(LIB)/mk/dpdk.mk

CXXFLAGS = -I$(LIB) $(DPDK_CFLAGS) -std=c++11
LDFLAGS  = $(DPDK_LDFLAGS)

all:
	$(CXX) $(CXXFLAGS) main.cc $(LDFLAGS)

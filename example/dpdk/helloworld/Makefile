
LIB = $(HOME)/git/slankdev/libslankdev
include $(LIB)/mk/dpdk.mk

CXXFLAGS = -I$(LIB) $(DPDK_CFLAGS) -std=c++11 -Wno-format-security
LDFLAGS  = $(DPDK_LDFLAGS)

all:
	$(CXX) $(CXXFLAGS) main.cc $(LDFLAGS)


DPDK_CFLAGS = -I$(RTE_SDK)/$(RTE_TARGET)/include
DPDK_LDFLAGS = -L$(RTE_SDK)/$(RTE_TARGET)/lib -ldpdk -lpthread -ldl -lrt

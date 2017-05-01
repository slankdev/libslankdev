
LIB = /home/slank/git/slankdev/libslankdev
include $(LIB)/mk/dpdk.mk
CXXFLAGS += $(DPDK_CFLAGS) -I$(LIB) -std=c++11
LDFLAGS  += $(DPDK_LDFLAGS)


all:
	$(CXX) $(CXXFLAGS) main.cc $(LDFLAGS)


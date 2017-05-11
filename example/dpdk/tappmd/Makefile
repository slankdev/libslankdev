
LIB = /home/slank/git/slankdev/libslankdev
include $(LIB)/mk/dpdk.mk
CXXFLAGS = -std=c++11 -I$(LIB) $(DPDK_CFLAGS)
LDFLAGS  = $(DPDK_LDFLAGS)

all:
	$(CXX) $(CXXFLAGS) main.cc $(LDFLAGS)

run:
	sudo ./a.out --vdev=net_tap0,iface=tap0


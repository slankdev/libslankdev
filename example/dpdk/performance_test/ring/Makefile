
LIB = /home/slank/git/slankdev/libslankdev
include $(LIB)/mk/dpdk.mk

CXXFLAGS = -I$(LIB) $(DPDK_CFLAGS) -std=c++11 -Wno-format-security -O0 -g
LDFLAGS  = $(DPDK_LDFLAGS)

all:
	$(CXX) $(CXXFLAGS) test1.cc -o test1.out $(LDFLAGS)
	$(CXX) $(CXXFLAGS) test2.cc -o test2.out $(LDFLAGS)

clean:
	rm -rf *.out


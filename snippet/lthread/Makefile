
LIBSLANKDEV := /home/slank/git/slankdev/libslankdev
CXXFLAGS += -I$(LIBSLANKDEV) -std=c++11 -Wno-format-security

include /home/slank/git/slankdev/libslankdev/mk/dpdk.mk
CXXFLAGS += $(DPDK_CFLAGS) -fsanitize=address
LDFLAGS  += $(DPDK_LDFLAGS)




all:
	$(CXX) $(CXXFLAGS) main.cc $(LDFLAGS) -llthread



LIB = /home/slank/git/libslankdev
LIB2 = /home/slank/git/libpgen
# include $(LIB)/mk/dpdk.mk

CXXFLAGS += -I$(LIB) -I$(LIB2) -std=c++11 -Wall -Wextra
LDFLAGS  +=

all:
	$(CXX) $(CXXFLAGS) main.cc $(LDFLAGS)

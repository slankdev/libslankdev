
SLANKLIB = ../../../

include $(SLANKLIB)/mk/dpdk.mk
DPDK_PATH := $(RTE_SDK)/$(RTE_TARGET)

CXXFLAGS = -I$(SLANKLIB) -std=c++11 -g
CXXFLAGS += -g -O0 $(DPDK_CFLAGS)
LDFLAGS  += $(DPDK_LDFLAGS)

all:
	$(CXX) $(CXXFLAGS) main.cc

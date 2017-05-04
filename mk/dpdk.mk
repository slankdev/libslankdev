
ifeq ($(RTE_SDK),)
$(error "Please define RTE_SDK environment variable")
endif
ifeq ($(RTE_TARGET),)
$(error "Please define RTE_TARGET environment variable")
endif

DPDK_PATH := $(RTE_SDK)/$(RTE_TARGET)

# no -mssse3 version always compile error....
DPDK_CFLAGS = \
	 -I$(DPDK_PATH)/include \
	 -include $(DPDK_PATH)/include/rte_config.h \
	 -mssse3

DPDK_LDFLAGS += \
	-Wl,--no-as-needed \
	-Wl,-export-dynamic \
	-L$(DPDK_PATH)/lib \
	-lpthread -ldl -lrt -lm -lpcap \
	-Wl,--whole-archive -Wl,--start-group \
	-ldpdk \
	-Wl,--end-group -Wl,--no-whole-archive



prefix = ../
TARGET = a.out
SRCS   = main.cc
OBJS   = $(SRCS:.cc=.o)
include $(prefix)/mk/vars.mk
CXXFLAGS += -Wno-format-security
LDFLAGS +=


all: $(TARGET)
main.o: system.h main.cc types.h port.h cpu.h ring.h dpdk_struct_utils.h

clean:
	$(RM) $(TARGET) $(OBJS)

run: run_native

run_native:
	sudo ./a.out

run_dummy:
	sudo ./a.out   \
		--vdev=eth_null0  \
		--vdev=eth_null1

gdb:
	sudo gdb --args a.out \
		--vdev=eth_null0  \
		--vdev=eth_null1

# run:
# 	sudo ./a.out \
# 		--vdev=eth_pcap0,iface=lo
#
# gdb:
# 	sudo gdb --args a.out \
# 		--vdev=eth_pcap0,iface=lo \
# 		--vdev=eth_null3

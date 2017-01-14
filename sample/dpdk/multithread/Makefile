

prefix = ../
TARGET = a.out
SRCS   = main.cc
OBJS   = $(SRCS:.cc=.o)
include $(prefix)/mk/vars.mk
LDFLAGS +=


all: $(TARGET)
main.o: system.h main.cc thread.h types.h port.h cpu.h ring.h util.h

clean:
	$(RM) $(TARGET) $(OBJS)

run:
	sudo ./a.out -c 0x1f

gdb:
	sudo gdb --args a.out -c 0x1f

# run:
# 	sudo ./a.out \
# 		--vdev=eth_pcap0,iface=lo
#
# gdb:
# 	sudo gdb --args a.out \
# 		--vdev=eth_pcap0,iface=lo \
# 		--vdev=eth_null3

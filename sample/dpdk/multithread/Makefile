

prefix = ../
TARGET = a.out
SRCS   = main.cc
OBJS   = $(SRCS:.cc=.o)
include $(prefix)/mk/vars.mk
LDFLAGS +=


all: $(TARGET)

clean:
	$(RM) $(TARGET) $(OBJS)


run:
	sudo ./a.out \
		--vdev=eth_pcap0,iface=eth0


runtest:
	sudo ./a.out \
		--vdev=eth_null0 \
		--vdev=eth_null1 \
		--vdev=eth_null2 \
		--vdev=eth_null3



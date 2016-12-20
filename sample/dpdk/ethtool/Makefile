

prefix = ../
TARGET = a.out
SRCS   = main.cc
OBJS   = $(SRCS:.cc=.o)
include $(prefix)/mk/vars.mk
LDFLAGS += -lreadline


# INTERFACE0 = eth1
INTERFACE0 = vboxnet1
INTERFACE1 = lo


all: $(TARGET)

clean:
	rm -f *.o


run:
	sudo ./a.out \
		--vdev=eth_pcap0,iface=$(INTERFACE0) \
		--vdev=eth_null0

runtest:
	sudo ./a.out \
		--vdev=eth_null0 \
		--vdev=eth_null1 \
		--vdev=eth_null2 \
		--vdev=eth_null3



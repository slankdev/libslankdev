
LIBSLANKDEV := $(HOME)/git/slankdev/libslankdev
CXXFLAGS += -I$(LIBSLANKDEV) -std=c++11

.cc.out:
	@echo "CXX $@"
	@$(CXX) $(CXXFLAGS) $< -o $@

SRC = \
			pkt_send.cc \
			bin_send.cc \
			create_tap.cc
EXE = $(SRC:.cc=.out)

all: $(EXE)

clean:
	rm -rf $(EXE)

install:
	cp packet_sender.out /usr/local/bin
	cp create_tap.out    /usr/local/bin


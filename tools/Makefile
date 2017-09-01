
LIBSLANKDEV := $(HOME)/git/libslankdev
CXXFLAGS += -I$(LIBSLANKDEV) -std=c++11
INSTALL_PATH = /usr/local/bin/

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
	cp pkt_send.out   $(INSTALL_PATH)
	cp bin_send.out   $(INSTALL_PATH)
	cp create_tap.out $(INSTALL_PATH)


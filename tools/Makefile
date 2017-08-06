
LIBSLANKDEV := $(HOME)/git/slankdev/libslankdev
CXXFLAGS += -I$(LIBSLANKDEV) -std=c++11

.cc.out:
	@echo "CXX $@"
	@$(CXX) $(CXXFLAGS) $< -o $@

SRC = \
			packet_sender.cc \
			create_tap.cc
EXE = $(SRC:.cc=.out)

all: $(EXE)

clean:
	rm -rf $(EXE)


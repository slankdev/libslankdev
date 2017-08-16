
CXX       = g++
CXXFLAGS  = -std=c++11 -I../
CXXFLAGS += -Wextra -Wall -Wno-format-security
ifeq ($(CXX), clang++)
CXXFLAGS += -Weverything
endif

LDFLAGS += -lpcap

.cc.out:
	@echo "CXX $@"
	@$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

SRC = test_include.cc    \
			test_mac2vendor.cc \
			test_csv.cc        \
			test_string.cc     \
			test_dns.cc
OBJ = $(SRC:.cc=.out)

all: $(OBJ)
	@echo "Check is done!!"

clean:
	rm -rf *.o *.out *.a


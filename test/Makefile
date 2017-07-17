
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

SRC = main.cc
OBJ = $(SRC:.cc=.out)

all: $(OBJ)
	@echo check C++ syntax...
	@$(CXX) $(CXXFLAGS) main.cc $(LDFLAGS)
	@echo check C++ syntax... OK



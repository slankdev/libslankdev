
CXX       = g++
CXXFLAGS  = -std=c++11 -I../
CXXFLAGS += -Wextra -Wall -Wno-format-security
ifeq ($(CXX), clang++)
CXXFLAGS += -Weverything
endif

all:
	@echo check C++ syntax...
	@$(CXX) $(CXXFLAGS) main.cc
	@echo check C++ syntax... OK



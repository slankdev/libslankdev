
export INSTALL_LIB_DIR := /usr/local/lib
export INSTALL_HDR_DIR := /usr/local/include
INCLUDE_DIR = include

export CPP             := g++
export CPPFLAGS        := -Wextra -g3  -O0 -std=c++11
export AR              := ar
export RANLIB          := ranlib
export MAKE            := make
export CP              := cp -rf
export RM              := rm -rf
export MKDIR           := mkdir -p

MAKEFLAGS += --no-print-directory

all:
	@echo syntax check
	@g++ -std=c++11 main.cc -Iinclude

build-test:
	$(MAKE) -C test

install: all
	$(CP) include/slankdev.h $(INSTALL_HDR_DIR)
	$(CP) include/slankdev   $(INSTALL_HDR_DIR)

uninstall:
	$(RM) $(INSTALL_HDR_DIR)/slankdev.h
	$(RM) $(INSTALL_HDR_DIR)/slankdev


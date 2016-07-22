
export INSTALL_LIB_DIR := /usr/local/lib
export INSTALL_HDR_DIR := /usr/local/include
INCLUDE_DIR = include

include common.mk

TARGET = libslankdev.a
SRC = src/util.cc  \
	  src/system.cc \
	  src/intfd.cc  
OBJ = $(SRC:.cc=.o)

.cc.o: 
	@echo [CXX] $@
	@$(CPP) $(CPPFLAGS) -g -c $< -o $@  -I$(INCLUDE_DIR)





all: $(TARGET)

libslankdev.a: $(OBJ)
	@rm -f $@
	$(AR) rc $@ $(OBJ)
	$(RANLIB) $@

build-test:
	$(MAKE) -C test

clean:
	$(RM) libslankdev.a
	$(RM) $(OBJ)
	$(MAKE) -C test clean

install: all
	$(CP) libslankdev.a  $(INSTALL_LIB_DIR)
	$(CP) include/slankdev.h $(INSTALL_HDR_DIR)
	$(CP) include/slankdev   $(INSTALL_HDR_DIR)

uninstall:
	$(RM) $(INSTALL_LIB_DIR)/libslankdev.a
	$(RM) $(INSTALL_HDR_DIR)/slankdev.h
	$(RM) $(INSTALL_HDR_DIR)/slankdev


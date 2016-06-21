
export INSTALL_LIB_DIR := /usr/local/lib
export INSTALL_HDR_DIR := /usr/local/include
INCLUDE_DIR = include

include common.mk

SRC = src/util.cc  \
	  src/system.cc \
	  src/intfd.cc  \
	  src/gns.cc  \
	  src/pollfd.cc 
OBJ = $(SRC:.cc=.o)

.cc.o: 
	$(CPP) $(CPPFLAGS) -c $< -o $@  -I$(INCLUDE_DIR)


all: libslankdev.a






all: libslankdev.a

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


install:
	$(CP) libslankdev.a  $(INSTALL_LIB_DIR)
	$(CP) include/slankdev.h $(INSTALL_HDR_DIR)
	$(CP) include/slankdev   $(INSTALL_HDR_DIR)


uninstall:
	$(RM) $(INSTALL_LIB_DIR)/libslankdev.a
	$(RM) $(INSTALL_HDR_DIR)/slankdev.h
	$(RM) $(INSTALL_HDR_DIR)/slankdev


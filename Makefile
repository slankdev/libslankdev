
export INSTALL_LIB_DIR := /usr/local/lib
export INSTALL_HDR_DIR := /usr/local/include

include common.mk


all:
	$(MAKE) -C src
	$(MAKE) -C test


clean:
	$(MAKE) -C src  clean
	$(MAKE) -C test clean


install:
	$(CP) src/libslankdev.a  $(INSTALL_LIB_DIR)
	$(CP) include/slankdev.h $(INSTALL_HDR_DIR)
	$(CP) include/slankdev   $(INSTALL_HDR_DIR)


uninstall:
	$(RM) $(INSTALL_LIB_DIR)/libslankdev.a
	$(RM) $(INSTALL_HDR_DIR)/slankdev.h
	$(RM) $(INSTALL_HDR_DIR)/slankdev


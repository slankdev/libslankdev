
INSTALL_PATH := /usr/local/include
MAKEFLAGS += --no-print-directory

def:
	@$(MAKE) -C test

install:
	@echo install to $(INSTALL_PATH)...
	@cp -rf slankdev $(INSTALL_PATH)
	@echo install to $(INSTALL_PATH)... OK

uninstall:
	@echo uninstall to rm "$(INSTALL_PATH)/slankdev"...
	@rm -rf $(INSTALL_PATH)/slankdev
	@echo uninstall to rm "$(INSTALL_PATH)/slankdev"... OK


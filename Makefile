
PREFIX       = /usr/local
MANPREFIX    = $(PREFIX)/man
INSTALL_PATH = $(PREFIX)/include

MAKEFLAGS += --no-print-directory

def:
	@$(MAKE) -C test

clean:
	@$(MAKE) -C test clean

doc:
	@$(MAKE) -C docs

install:
	@echo install to $(INSTALL_PATH)...
	@cp -rf slankdev $(INSTALL_PATH)
	@echo install to $(INSTALL_PATH)... OK

uninstall:
	@echo uninstall to rm "$(INSTALL_PATH)/slankdev"...
	@rm -rf $(INSTALL_PATH)/slankdev
	@echo uninstall to rm "$(INSTALL_PATH)/slankdev"... OK

link-install:
	@echo create symboric link to $(INSTALL_PATH)...
	@ln -s `pwd`/slankdev $(INSTALL_PATH)/slankdev
	@echo create symboric link to $(INSTALL_PATH)... OK

# install_doc:
# 	@echo installing manual page to $(MANPREFIX)/man3
# 	@mkdir -p $(MANPREFIX)/man3
# 	cp doc/man/man3/* $(MANPREFIX)/man3/
# 	@chmod 644 ${DESTDIR}${MANPREFIX}/man1/dwm.1

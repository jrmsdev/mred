PREFIX ?= /opt/pkg

.PHONY: all
all: ./build/mred.bin

./build/mred.bin:
	@mkdir -vp ./build
	@cp -f Makefile.build ./build/Makefile
	@(cd ./build && $(CC) -E -MM ../*.c >>Makefile)
	@$(MAKE) -C build build

.PHONY: clean
clean:
	@rm -vrf .do-install ./build

.PHONY: install
install: .do-install

.do-install: ./build/mred.bin
	@mkdir -vp $(DESTDIR)$(PREFIX)/bin
	@mkdir -vp $(DESTDIR)$(PREFIX)/share/licenses/mred
	@install -v -m 0555 ./build/mred.bin $(DESTDIR)$(PREFIX)/bin/mred
	@install -v -m 0444 LICENSE $(DESTDIR)$(PREFIX)/share/licenses/mred
	@touch .do-install

.PHONY: uninstall
uninstall:
	@rm -vf $(DESTDIR)$(PREFIX)/bin/mred
	@rm -vrf $(DESTDIR)$(PREFIX)/share/licenses/mred

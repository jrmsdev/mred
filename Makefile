PREFIX ?= /opt/pkg


.PHONY: all
all: build


.PHONY: build
build:
	@mkdir -vp ./build
	@(cd ./build && CC=$(CC) ../scripts/gen-objsmk.sh >objs.mk)
	@(cd ./build && $(MAKE) -f ../build.mk build)


.PHONY: clean
clean:
	@rm -vrf .do-install ./build ./tests/*.test ./tests/*.vgout


.PHONY: install
install: .do-install


.do-install: ./build/mred.bin
	@mkdir -vp $(DESTDIR)$(PREFIX)/bin
	@mkdir -vp $(DESTDIR)$(PREFIX)/share/licenses/mred
	@install -v -m 0555 ./build/mred.bin $(DESTDIR)$(PREFIX)/bin/mred
	@install -v -m 0444 LICENSE $(DESTDIR)$(PREFIX)/share/licenses/mred
	@touch .do-install


./build/mred.bin: build


.PHONY: uninstall
uninstall:
	@rm -vf $(DESTDIR)$(PREFIX)/bin/mred
	@rm -vrf $(DESTDIR)$(PREFIX)/share/licenses/mred


.PHONY: check
check:
	@(cd ./tests && MAKE=$(MAKE) ../scripts/run-tests.sh)


.PHONY: check-valgrind
check-valgrind:
	@(cd ./tests && MAKE=$(MAKE) ../scripts/run-tests.sh --valgrind)

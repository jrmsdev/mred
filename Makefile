PREFIX ?= /opt/pkg
TEST_CFLAGS := -D__ALLOW_NOTTY -O0 -ggdb


.PHONY: all
all: build


.PHONY: build
build:
	@mkdir -vp ./build
	@(cd ./build && CC=$(CC) ../scripts/gen-objsmk.sh >objs.mk)
	@(cd ./build && $(MAKE) -f ../build.mk build)


.PHONY: build.test
build.test:
	@mkdir -vp ./build.test
	@(cd ./build.test && CC=$(CC) ../scripts/gen-objsmk.sh >objs.mk)
	@(cd ./build.test && \
		$(MAKE) -f ../build.mk build MRED_CFLAGS='$(TEST_CFLAGS)')


.PHONY: clean
clean:
	@rm -vrf .do-install ./build ./build.test ./tests/*.bin ./tests/*.vgout*
	@rm -vrf ./t/t????_*/stdout.*


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
check: build.test
	#@(cd ./tests && MAKE=$(MAKE) ../scripts/run-tests.sh)
	@(cd ./t && ../scripts/run_T.sh)


.PHONY: check-valgrind
check-valgrind: build.test
	#@(cd ./tests && MAKE=$(MAKE) ../scripts/run-tests.sh --valgrind)
	@(cd ./t && ../scripts/run_T.sh --valgrind)

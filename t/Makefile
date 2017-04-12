TEST_CFLAGS := -D__ALLOW_NOTTY -O0 -ggdb

.PHONY: check
check: build
	@./run.sh

.PHONY: check-valgrind
check-valgrind: build
	@./run.sh --valgrind

.PHONY: build
build:
	@mkdir -vp ../build.test
	@(cd ../build.test && CC=$(CC) ../scripts/gen-objsmk.sh >objs.mk)
	@(cd ../build.test && \
		$(MAKE) -f ../build.mk build MRED_CFLAGS='$(TEST_CFLAGS)')

.PHONY: clean
clean:
	@rm -vf t????_*/stdout.*

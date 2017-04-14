.PHONY: check
check: build.test
	@../scripts/run_T.sh

.PHONY: check-valgrind
check-valgrind: build.test
	@../scripts/run_T.sh --valgrind

.PHONY: build.test
build.test:
	@(cd .. && $(MAKE) build.test)

.PHONY: clean
clean:
	@rm -vf ./t????_*/stdout.* ./t????_*/stderr.*
	@rm -vrf ./t_compile/*.bin ./t_compile/*.vgout*

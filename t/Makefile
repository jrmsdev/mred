.PHONY: check
check: build
	@./run.sh


.PHONY: build
build:
	@mkdir -vp ../build.test
	@(cd ../build.test && CC=$(CC) ../scripts/gen-objsmk.sh >objs.mk)
	@(cd ../build.test && \
		$(MAKE) -f ../build.mk build MRED_DEBUG=-D__ALLOW_NOTTY)

.PHONY: clean
clean:
	@rm -vf t????_*/stdout.*
	@rm -rfv ../build.test

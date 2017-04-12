.PHONY: check
check:
	@$(MAKE) -C .. clean build MRED_DEBUG=-D__ALLOW_NOTTY >/dev/null
	@./run.sh

.PHONY: clean
clean:
	@rm -vf t????_*/stdout.*

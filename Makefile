PREFIX ?= /opt/pkg
CFLAGS += -Wall -Wextra -pedantic -std=c99
OBJS := main.o terminal.o input.o output.o buffer.o

.PHONY: all
all: mred.bin

$(OBJS): mred.h

.c.o:
	$(CC) $(CFLAGS) -c -fPIC -o $@ $<

mred.bin: $(OBJS)
	$(CC) $(CFLAGS) -o mred.bin $(OBJS)

.PHONY: clean
clean:
	@rm -vf mred $(OBJS)

.PHONY: install
install: mred.bin
	@mkdir -vp $(DESTDIR)$(PREFIX)/bin
	@install -v -m 0555 mred.bin $(DESTDIR)$(PREFIX)/bin/mred

.PHONY: uninstall
uninstall:
	@rm -vf $(DESTDIR)$(PREFIX)/bin/mred

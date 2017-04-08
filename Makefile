CFLAGS += -Wall -Wextra -pedantic -std=c99
OBJS := main.o terminal.o input.o output.o buffer.o
PREFIX ?= /opt/pkg

.PHONY: all
all: mred

$(OBJS): mred.h

.c.o:
	$(CC) $(CFLAGS) -c -fPIC -o $@ $<

mred: $(OBJS)
	$(CC) $(CFLAGS) -o mred $(OBJS)

.PHONY: clean
clean:
	@rm -vf mred $(OBJS)

.PHONY: install
install: mred
	@mkdir -vp $(DESTDIR)$(PREFIX)/bin
	@install -v -m 0555 mred $(DESTDIR)$(PREFIX)/bin/mred

.PHONY: uninstall
uninstall:
	@rm -vf $(DESTDIR)$(PREFIX)/bin/mred

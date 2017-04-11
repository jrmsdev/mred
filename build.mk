CFLAGS += -Wall -Wextra -pedantic -std=c99
BUILD_OBJ := $(CC) $(CFLAGS) -c -fPIC
OBJS := main.o terminal.o input.o output.o buffer.o file_io.o row_ops.o \
	ed_ops.o find.o syntax_hl.o

.PHONY: build
build: mred.bin

mred.bin: $(OBJS)
	$(CC) $(CFLAGS) -o mred.bin $(OBJS)

.PHONY: clean
clean:
	@rm -vf mred.bin *.o

include ./objs.mk

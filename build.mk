CFLAGS += -Wall -Wextra -pedantic -std=c11 $(MRED_DEBUG)
OBJ_CFLAGS := -c -fPIC
OBJS := main.o terminal.o input.o output.o buffer.o file_io.o row_ops.o \
	ed_ops.o find.o syntax_hl.o

.PHONY: build
build: mred.bin

mred.bin: $(OBJS)
	$(CC) $(CFLAGS) -o mred.bin $(OBJS)

include ./objs.mk

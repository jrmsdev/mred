CFLAGS := -Wall -Wextra -pedantic -pipe -march=native -std=c11 -O0 -ggdb

.SUFFIXES:
.SUFFIXES: .bin .c

.c.bin:
	@$(CC) $(CFLAGS) -o $@ $<

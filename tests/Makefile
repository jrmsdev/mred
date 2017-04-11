CFLAGS := -Wall -Wextra -pedantic -pipe -march=native -std=c11 -O0 -ggdb

.SUFFIXES:
.SUFFIXES: .test .c

.c.test:
	@$(CC) $(CFLAGS) -o $@ $<

#!/bin/sh

test -z "$CC" && {
    echo "$0: CC env var not set" >&2
    exit 1
}

which $CC >/dev/null 2>/dev/null || {
    echo "$0: $CC command not found!" >&2
    exit 2
}

test -d ../src || {
    echo "$0: src dir not found" >&2
    exit 3
}

$CC -E -MM ../src/*.c | while read objline
do
    printf '%s\n\t$(CC) $(CFLAGS) $(OBJ_CFLAGS) -o $@ $<\n\n' "$objline"
done

exit 0

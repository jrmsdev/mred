#!/bin/sh

which $CC >/dev/null 2>/dev/null || {
    echo "CC env var not set or command not found!" >&2
    exit 1
}

$CC -E -MM ../src/*.c | while read objline
do
    printf '%s\n\t$(BUILD_OBJ) -o $@ $<\n\n' "$objline"
done

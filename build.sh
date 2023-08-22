#!/bin/sh

CC="cc"
CFLAGS="-std=c99 -Wall -Wextra -pedantic -I ./include -lm"
BIN="octet"
SRC="$(find ./src -type f -name '*.c' -printf '%h/%f ')\
     $(find ./tests -type f -name '*.c' -printf '%h/%f ')"

if [ "$1" = "TEST" ]; then
    CFLAGS="${CFLAGS} -ggdb"
    exec ${CC} ${CFLAGS} ${SRC} -o ${BIN}
fi

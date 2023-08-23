#!/bin/sh

CC="gcc"
CFLAGS="-std=c99 -Wall -Wextra -pedantic -I ./include"
LDFLAGS="-lm"
BIN="octet"
SRC="$(find ./src -type f -name '*.c' -printf '%h/%f ') ./example.c"

if [ "$1" = "TEST" ]; then
    CFLAGS="${CFLAGS} -ggdb"
    BIN="octet_test"
    SRC="$(find ./src -type f -name '*.c' -printf '%h/%f ')\
         $(find ./tests -type f -name '*.c' -printf '%h/%f ')"
fi

if [ "$1" = "LIB" ]; then
    exec ${CC} ${CFLAGS} ${LDFLAGS} -ggdb -fPIC -shared ${SRC} -o liboctet.so
    return 0
fi

exec ${CC} ${CFLAGS} ${LDFLAGS} ${SRC} -o ${BIN}

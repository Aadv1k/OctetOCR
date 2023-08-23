#!/bin/sh

CC="cc"
CFLAGS="-std=c99 -Wall -Wextra -pedantic -I ./include -lm"
BIN="octet"
LIB="-lm"
SRC="$(find ./src -type f -name '*.c' -printf '%h/%f ')\
     $(find ./tests -type f -name '*.c' -printf '%h/%f ')"

if [ "$1" = "TEST" ]; then
    CFLAGS="${CFLAGS} -ggdb"
    BIN="octet_test"
fi

exec ${CC} ${CFLAGS} ${LIB} ${SRC} -o ${BIN}

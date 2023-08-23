#!/bin/sh

CC="gcc"
CFLAGS="-std=c99 -Wall -Wextra -pedantic -I ./include"
BIN="octet"
LIB="-lm"
SRC="$(find ./src -type f -name '*.c' -printf '%h/%f ')" 

if [ "$1" = "TEST" ]; then
    CFLAGS="${CFLAGS} -ggdb"
    BIN="octet_test"
    SRC+="$(find ./tests -type f -name '*.c' -printf '%h/%f ')"
fi

${CC} ${CFLAGS} ${LIB} ${SRC} -o ${BIN}

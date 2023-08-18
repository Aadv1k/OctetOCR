@echo off

SET CC=gcc
SET CFLAGS=-Wall -Wextra -std=c99
SET SRC=main.c
SET EXEC=octet.exe


%CC% %CFLAGS% %SRC% -o %EXEC%

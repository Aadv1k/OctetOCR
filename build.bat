@echo off

SET CC=gcc
SET CFLAGS=-std=c99
SET SRC=src\*.c main.c
SET EXEC=octet.exe
SET INC_PATH=.\include 

IF "%1%" == "TEST" (
  SET SRC=src\*.c tests\*.c tests\munit\munit.c
  SET EXEC=octet_test.exe
)

%CC% %CFLAGS% -I %INC_PATH% %SRC% -o %EXEC%

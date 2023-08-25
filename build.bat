@echo off

SET CC=gcc
SET CFLAGS=-std=c99 -Wall -Wextra -pedantic
SET SRC=src\*.c example.c
SET EXEC=octet.exe
SET INC_PATH=.\include 

IF "%1%" == "DEBUG" (
  SET EXEC=octet_debug.exe
  SET CFLAGS=%CLFAGS% -ggdb
)


IF "%1%" == "TEST" (
  SET SRC=src\*.c tests\*.c tests\munit\munit.c
  SET EXEC=octet_test.exe
  SET CFLAGS=%CLFAGS% -ggdb
)

IF "%1%" == "LIB" (
  SET SRC=src\*.c
  %CC% -std=c99 -ggdb -fPIC -shared -I %INC_PATH% %SRC% -o liboctet.dll
  exit 0
)

if "%1%" == "CLEAN" (
    del "octet.exe"
    del "octet_test.exe"
    del "liboctet.dll"
    exit 0
)

%CC% %CFLAGS% -I %INC_PATH% %SRC% -o %EXEC%

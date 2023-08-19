nd-@echo off

SET CC=gcc
SET CFLAGS=-std=c99
SET SRC=src\*.c main.c
SET EXEC=octet.exe
SET INC_PATH=.\include 


%CC% %CFLAGS% -I %INC_PATH% %SRC% -o %EXEC%

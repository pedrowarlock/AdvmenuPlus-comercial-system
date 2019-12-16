@echo off
copy Makefile.usr Makefile
path ..\mingw\bin
mingw32-make.exe
pause
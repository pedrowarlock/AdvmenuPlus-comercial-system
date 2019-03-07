@echo off



set DJGPP=\COMPILADOR\DJGPP\DJGPP.ENV
PATH \COMPILADOR\DJGPP\BIN;%PATH%

stubedit \COMPILADOR\djgpp\bin\gcc.exe bufsize=32k
stubedit \COMPILADOR\djgpp\bin\gpp.exe bufsize=32k
stubedit \COMPILADOR\djgpp\bin\ld.exe bufsize=32k
stubedit \COMPILADOR\djgpp\bin\make.exe bufsize=32k
stubedit \COMPILADOR\djgpp\lib\gcc-lib\djgpp\3.23\collect2.exe bufsize=32k

make

pause
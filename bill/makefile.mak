# Created by IBM WorkFrame/2 MakeMake at 14:18:55 on 10/29/95
#
# This makefile should be run in the following directory:
#   f:\c_dev\bill\bill
#
# The actions included in this makefile are:
#   COMPILE::C Set ++ Compile
#   LINK::Link

.all: \
  .\bill.exe

.SUFFIXES:

.SUFFIXES: .c

.c.obj:
      @echo WF::COMPILE::C Set ++ Compile
      icc.exe /Tl- /Ss /Ti /G4 /C %s

.\bill.exe: \
    .\bill.obj \
    {$(LIB)}bill.def \
    makefile.mak
      @echo WF::LINK::Link
      link386.exe @<<
         /PM:VIO /F /NON /PACKD: /E +
        .\bill.obj +
        ..\exitcode\exitcode.obj +
        ..\lexical\lexical.obj +
        ..\parser\parser.obj +
        ..\pcode\pcode.obj +
        ..\stackman\stackman.obj +
        ..\symtable\symtable.obj
        bill.exe
        
        
        bill.def;
<<

.\bill.obj: \
    f:\c_dev\bill\bill\bill.c \
    ..\parser\parser.h \
    ..\stackman\stackman.h \
    ..\global\global.h \
    ..\global\error.h \
    ..\exitcode\exitcode.h \
    makefile.mak


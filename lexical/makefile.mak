# Created by IBM WorkFrame/2 MakeMake at 14:15:14 on 10/29/95
#
# This makefile should be run in the following directory:
#   f:\c_dev\bill\lexical
#
# The actions included in this makefile are:
#   COMPILE::C Set ++ Compile

.all: \
  .\lexical.obj

.SUFFIXES:

.SUFFIXES: .c

.c.obj:
      @echo WF::COMPILE::C Set ++ Compile
      icc.exe /Tl- /Ss /Ti /G4 /C %s

.\lexical.obj: \
    f:\c_dev\bill\lexical\lexical.c \
    ..\lexical\lexical.hi \
    ..\lexical\lexical.h \
    ..\symtable\symtable.h \
    ..\global\global.h \
    ..\lexical\lex_ids.h \
    ..\symtable\symt_ids.h \
    ..\global\error.h \
    ..\exitcode\exitcode.h \
    makefile.mak


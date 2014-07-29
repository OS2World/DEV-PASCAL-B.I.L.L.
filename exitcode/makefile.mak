# Created by IBM WorkFrame/2 MakeMake at 14:15:25 on 10/29/95
#
# This makefile should be run in the following directory:
#   f:\c_dev\bill\exitcode
#
# The actions included in this makefile are:
#   COMPILE::C Set ++ Compile

.all: \
  .\exitcode.obj

.SUFFIXES:

.SUFFIXES: .c

.c.obj:
      @echo WF::COMPILE::C Set ++ Compile
      icc.exe /Tl- /Ss /Ti /G4 /C %s

.\exitcode.obj: \
    f:\c_dev\bill\exitcode\exitcode.c \
    ..\exitcode\exitcode.hi \
    ..\\exitcode\\exitcode.h \
    ..\\lexical\\lexical.h \
    ..\\symtable\\symtable.h \
    ..\\parser\\parser.h \
    ..\global\global.h \
    ..\lexical\lex_ids.h \
    ..\symtable\symt_ids.h \
    ..\global\error.h \
    makefile.mak


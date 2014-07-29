# Created by IBM WorkFrame/2 MakeMake at 14:15:36 on 10/29/95
#
# This makefile should be run in the following directory:
#   f:\c_dev\bill\parser
#
# The actions included in this makefile are:
#   COMPILE::C Set ++ Compile

.all: \
  .\parser.obj

.SUFFIXES:

.SUFFIXES: .c

.c.obj:
      @echo WF::COMPILE::C Set ++ Compile
      icc.exe /Tl- /Ss /Ti /G4 /C %s

.\parser.obj: \
    f:\c_dev\bill\parser\parser.c \
    ..\parser\parser.hi \
    ..\parser\parser.h \
    ..\parser\pars_ids.h \
    ..\lexical\lexical.h \
    ..\symtable\symtable.h \
    ..\pcode\pcode_id.h \
    ..\pcode\pcode.h \
    ..\global\global.h \
    ..\lexical\lex_ids.h \
    ..\symtable\symt_ids.h \
    ..\global\error.h \
    ..\exitcode\exitcode.h \
    makefile.mak


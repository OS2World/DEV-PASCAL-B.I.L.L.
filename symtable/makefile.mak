# Created by IBM WorkFrame/2 MakeMake at 14:15:47 on 10/29/95
#
# This makefile should be run in the following directory:
#   f:\c_dev\bill\symtable
#
# The actions included in this makefile are:
#   COMPILE::C Set ++ Compile

.all: \
  .\symtable.obj

.SUFFIXES:

.SUFFIXES: .c

.c.obj:
      @echo WF::COMPILE::C Set ++ Compile
      icc.exe /Tl- /Ss /Ti /G4 /C %s

.\symtable.obj: \
    f:\c_dev\bill\symtable\symtable.c \
    ..\symtable\symtable.hi \
    ..\symtable\symtable.h \
    ..\global\global.h \
    ..\symtable\symt_ids.h \
    ..\global\error.h \
    ..\exitcode\exitcode.h \
    makefile.mak


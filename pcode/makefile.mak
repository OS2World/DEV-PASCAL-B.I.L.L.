# Created by IBM WorkFrame/2 MakeMake at 14:15:57 on 10/29/95
#
# This makefile should be run in the following directory:
#   f:\c_dev\bill\pcode
#
# The actions included in this makefile are:
#   COMPILE::C Set ++ Compile

.all: \
  .\pcode.obj

.SUFFIXES:

.SUFFIXES: .c

.c.obj:
      @echo WF::COMPILE::C Set ++ Compile
      icc.exe /Tl- /Ss /Ti /G4 /C %s

.\pcode.obj: \
    f:\c_dev\bill\pcode\pcode.c \
    ..\pcode\pcode.hi \
    ..\pcode\pcode.h \
    ..\global\global.h \
    ..\global\error.h \
    ..\exitcode\exitcode.h \
    makefile.mak


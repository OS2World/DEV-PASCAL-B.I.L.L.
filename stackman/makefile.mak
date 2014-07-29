# Created by IBM WorkFrame/2 MakeMake at 14:16:13 on 10/29/95
#
# This makefile should be run in the following directory:
#   f:\c_dev\bill\stackman
#
# The actions included in this makefile are:
#   COMPILE::C Set ++ Compile

.all: \
  .\stackman.obj

.SUFFIXES:

.SUFFIXES: .c

.c.obj:
      @echo WF::COMPILE::C Set ++ Compile
      icc.exe /Tl- /Ss /Ti /G4 /C %s

.\stackman.obj: \
    f:\c_dev\bill\stackman\stackman.c \
    ..\pcode\pcode_id.h \
    makefile.mak


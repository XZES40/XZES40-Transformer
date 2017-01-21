#
# This Makefile is designed for a specific GLIB architecture.
#
GARCH = i486-slackware-linux
GVERS = 3.3.6

#
# These Macros define the necessary source trees required
# to find the Apache Source Code for debugging.
#
XALANCROOT := /src/apache/xalan-1_10/c
XERCESCROOT := /src/apache/xerces_2_7_0

#
# This is where the Apache (XALAN/XERCES) libraries reside
#

CPATH := /src/apache/include
# CPATH := /usr/local/include

#
# This is where to find the link loader libraries
#

LD_LIBRARY_PATH := /src/apache/lib:/usr/lib:/usr/local/lib:/lib:/usr/$(GARCH)/lib
# LD_LIBRARY_PATH := /usr/local/lib:/usr/lib:/lib:/usr/$(GARCH)/lib

CC = gcc
LD = ld

#
# These help find the C,C++ support objects that are required
# to support the main() program entry point and support the
# loading of dynamic libraries on GNU Linux platforms
#
# The Gnu link loader requires "--eh-frame-hdr" so that G++ exception handling
# works properly with the Apache XALAN/XERCES libraries.

XCRTB  = /usr/lib/crt1.o /usr/lib/crti.o /usr/lib/gcc-lib/$(GARCH)/$(GVERS)/crtbegin.o
XCRTE  = /usr/lib/gcc-lib/$(GARCH)/$(GVERS)/crtend.o /usr/lib/crtn.o
XLIBS  = -lxalan-c -lxalanMsg -lxerces-c
GLIBS  = -lgcc_s --no-as-needed -lc
DLINK  = -dynamic-linker /lib/ld-linux.so.2
LDOPTS = --eh-frame-hdr

DEBUG = -g
# DEBUG =

all:	xtest

clean:
	rm -f xtest.o
	rm -f orcvdate.o
	rm -f LEMSXpathInstaller.o
	rm -f LEMSXpConvertDate.o
	rm -f xtest

xtest.o: xtest.c LEMSXpath.h
	$(CC) -c $(DEBUG) xtest.c

orcvdate.o: orcvdate.c
	$(CC) -c $(DEBUG) orcvdate.c

LEMSXpathInstaller.o: LEMSXpathInstaller.cpp LEMSXpath.h
	$(CC) -c $(DEBUG) LEMSXpathInstaller.cpp

LEMSXpConvertDate.o: LEMSXpConvertDate.cpp LEMSXpConvertDate.hpp orcvdate.h
	$(CC) -c $(DEBUG) LEMSXpConvertDate.cpp

xtest: xtest.o LEMSXpathInstaller.o LEMSXpConvertDate.o orcvdate.o
	$(LD) -o $@ -Map xtest.map $(LDOPTS) $(DLINK) $(XCRTB) $+ $(XLIBS) $(GLIBS) $(XCRTE)


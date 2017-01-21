#!/bin/bash

#
# Build Apache Xerces (XML Parser) Libraries
#

if [ "$1" == "Debug" ] ; then
 DoDebug="T"
else
 DoDebug="F"
fi

export SourceDir=/data/src/xerces/c/trunk

if [ "$DoDebug" == "T" ] ; then
 export DestDir=/opt/debug
else
 export DestDir=/opt
fi

export PATH=$PATH:${SourceDir}

#
# We are to build in a directory outside of the source tree.
#

cd $HOME
if [ ! -d "build" ] ; then
  mkdir build
fi
cd build
if [ ! -d "Xerces-Build" ] ; then
  mkdir Xerces-Build
fi
cd Xerces-Build

#
# Configure the build system
#

if [ "$DoDebug" == "T" ] ; then
  configure CC="gcc -g" CXX="g++ -g" CFLAGS=-O2 CXXFLAGS=-O2 --srcdir=${SourceDir} --prefix=${DestDir}
else
  configure CC="gcc" CXX="g++" CFLAGS=-O3 CXXFLAGS=-O3 --srcdir=${SourceDir} --prefix=${DestDir}
fi

#
# Build the products
#

  make clean
  make

#
# Install the products - usually requires root
# for permissions to write.
#

  make install



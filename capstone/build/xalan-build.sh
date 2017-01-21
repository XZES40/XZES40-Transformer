#!/bin/bash

#
# Build Apache Xalan (XSLT Transform) Libraries
#

if [ "$1" == "Debug" ] ; then
 DoDebug="T"
 DBGTAG=-d
else
 DoDebug="F"
 DBGTAG=
fi

export SourceDir=/data/src/xalan/c/trunk

if [ "$DoDebug" == "T" ] ; then
 export DestDir=/opt/debug
else
 export DestDir=/opt
fi

export XERCESCROOT=${DestDir}
export XALANCROOT=${SourceDir}

export PATH=$PATH:${SourceDir}
export LD_LIBRARY_PATH=${DestDir}:$LD_LIBRARY_PATH

#
# We are to build in a directory outside of the source tree.
#

cd $HOME
if [ ! -d "build" ] ; then
  mkdir build
fi
cd build
if [ ! -d "Xalan-Build" ] ; then
  mkdir Xalan-Build
fi
cd Xalan-Build

runConfigure -p linux -c gcc -x g++ -P "${DestDir}" ${DBGTAG} -C "--srcdir=${SourceDir}"

make clean

# MAKE WHAT YOU NEED
# The standard make builds only the libxalan-c.so library and Xalan executable.
# The "samples" builds the sample applications.
# The "tests" builds the diagnostic tests

  make
  make samples
# make tests

# INSTALL WHAT YOU NEED
# The standard "install" installs only the liubxalan-c.,so and Xalan executable.
# The "install-extras" installs license and other documentation.
# The "install-package" installs linxalan-c.so Xalan execuable and extras.

  make install
# make install-extras
# make install-package

# Note: The "install-extras" are targeted to $(datadir)/doc/libxalan-c
# The standard $(datadir) = $(prefix)/share



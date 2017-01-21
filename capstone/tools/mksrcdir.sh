#!/bin/sh
#
# Prepared for OSU-CAPSTONE project.
# Prepared by: Steven J. Hathaway
#
# CAUTION: Users of this script should activate the "staff" security group.
# CAUTION: Users of this script should have umask 002 set instead of (022).
#
# This script populates working directories in /data/src/*
#
# The /data/ directory should be owned by "root:staff"
# The /data/ directory mode may be either 2775 or 3777,
#
# The /data/src/xalan/ is populated from the current subversion trunk
# The /data/src/xerces/ is populated from the current subversion trunk
# The /data/src/xalan-c-1.11/ is populated from the Apache xalan release
# The /data/src/xerces-c-3.1.4 is populated from the Apache xerces release
#
# Related scripts are found in /data/capstone/platform
#
# The /data/icu/ directories are extraction copies from /data/pkg/icu* packages.
#

umask 0002
MYDIR=`pwd`

if [ ! -d /data ] ; then
  echo "ERROR: /data directory does not exist"
  exit
fi
if [ ! -d /data/src ] ; then
  echo "INFO - Making Directory /data/src"
  mkdir /data/src
fi
if [ ! -d /data/src/xerces ] ; then
  echo "INFO - Making Directory /data/src/xerces"
  mkdir /data/src/xerces
fi
if [ ! -d /data/src/xerces/c ] ; then
  echo "INFO - Making Directory /data/src/xerces/c"
  mkdir /data/src/xerces/c
fi
if [ ! -d /data/src/xalan ] ; then
  echo "INFO - Making Directory /data/src/xalan"
  mkdir /data/src/xalan
fi
if [ ! -d /data/src/xalan/c ] ; then
  echo "INFO - Making Directory /data/src/xalan/c"
  mkdir /data/src/xalan/c
fi

echo "Directories for /data/src/* exist."

if [ ! -d /data/src/xerces/c/trunk -a -d /data/svn/xerces/c/trunk ] ; then
  echo "COPYING SVN FOR XERCES TO /data/src/xerces/c/trunk"
  cp -p -r /data/svn/xerces/c/trunk /data/src/xerces/c
  rm -rf /data/src/xerces/c/trunk/.svn
  echo XERCES Copy Complete
fi

if [ ! -d /data/src/xalan/c/trunk -a -d /data/svn/xalan/c/trunk ] ; then
  echo "COPYING SVN for XALAN TO /data/src/xalan/c/trunk"
  cp -p -r /data/svn/xalan/c/trunk /data/src/xalan/c
  rm -rf /data/src/xalan/c/trunk/.svn
  echo XALAN Copy Complete
fi

if [ ! -d /data/src/xerces-c-3.1.4 ] ; then
  echo "UNPACKING XERCES SOURCE TO /data/src/xerces-c-3.1.4"
  cd /data/src
  tar -xjf ../pkgs/xerces-c-3.1.4.tar.bz2
  echo "UNPACK XERCES SOURCE COMPLETE"
  cd $MYDIR
fi

if [ ! -d /data/src/xalan_c-1.11 ] ; then
  echo "UNPACKING XALAN SOURCE TO /data/src/xalan-c-1.11"
  cd /data/src
  tar -xzf ../pkgs/xalan_c-1.11-src.tar.gz
  echo "UNPACK XALAN SOURCE COMPLETE"
  cd $MYDIR
fi

if [ ! -f /data/src/xerces/c/trunk/config ] ; then
  echo "PREPARING XERCES FOR SOURCE BUILD"
  cd /data/src/xerces/c/trunk
  ./reconf
  echo "CONFIGURATION FOR XERCES SOURCE BUILD COMPLETE"
  cd $MYDIR
fi



 

#!/bin/sh
#
# Prepared for OSU-CAPSTONE project.
# Prepared by: Steven J. Hathaway
#
# CAUTION: Users of this script should activate the "staff" security group.
# CAUTION: Users of this script should have umask 002 set instead of (022).
#
# This script populates working directories in /data/icu/*
#
# The /data/ directory should be owned by "root:staff"
# The /data/ directory mode may be either 2775 or 3777,
#
# The /data/icu/icu4c-57_1-docs is populated from pkgs/icu4c-57_1-docs.zip
# The /data/icu/icu4c-57_1-data is populated from pkgs/icu4c-57_1-data.zip
# The /data/icu/icu4c-57_1-src is populated from pkgs/icu4c-57_1-src.zip
#
# Related scripts are found in /data/capstone/platform
#

umask 0002
MYDIR=`pwd`

if [ ! -d /data ] ; then
  echo "ERROR: /data directory does not exist"
  exit
fi
if [ ! -d /data/icu ] ; then
  echo "INFO - Making Directory /data/icu"
  mkdir /data/icu
fi
if [ ! -d /data/icu/icu4c-57_1-docs ] ; then
  echo "INFO - Making Directory /data/icu/icu4c-57_1-docs"
  mkdir /data/icu/icu4c-57_1-docs
  cd /data/icu/icu4c-57_1-docs
  unzip ../../pkgs/icu4c-57_1-docs.zip
fi
if [ ! -d /data/icu/icu4c-57_1-data ] ; then
  echo "INFO - Making Directory /data/icu/icu4c-57_1-data"
  mkdir /data/icu/icu4c-57_1-data
  cd /data/icu/icu4c-57_1-data
  unzip ../../pkgs/icu4c-57_1-data.zip
fi
if [ ! -d /data/icu/icu4c-57_1-src ] ; then
  echo "INFO - Making Directory /data/icu/icu4c-57_1-src"
  mkdir /data/icu/icu4c-57_1-src
  cd /data/icu/icu4c-57_1-src
  tar -zxvf ../../pkgs/icu4c-57_1-src.tgz
fi

echo "FIXUP WRITE PERMISSIONS FOR GROUP staff USAGE"

cd /data
find icu -exec chmod g+w {} \;

echo "FINISHED LOADING ICU LIBRARY SOURCES AND staff USAGE PERMISSIONS"

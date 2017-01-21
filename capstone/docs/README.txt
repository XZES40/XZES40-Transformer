OSU/ASF CAPSTONE 2016-17 PROJECT SOURCES
========================================

The /data/* path description

This directory is prepared for the OSU Capstone project by Steven J. Hathaway.

Users of this directory should have their group and umask permissions.

  newgrp staff
  umask 2

The /data/* contents are configured so that group "staff" has full access.

FILES:
  /root/.bashrc
    Add: umask 002
  $HOME/.bashrc
    Add: umask 002
  /etc/gshadow
    Add: login name list to group "staff"
    Keep: "staff" password locked
    This allows specific users to perform "newgrp staff" without password

Debian Linux distributions are also protecting /usr/local/* with 
group "staff" permissions.

/data/git = snapshot of the OSU Capstone GIT Repository
/data/svn/xerces/c/trunk = snapshot of the Apache xerces/c/trunk subversion
/data/svn/xalan/c/trunk = snapshot of the Apache xalan/c/trunk subversion
/data/pkgs/* = released product packages for xerces, xalan, and icu

script: mksrcdir.sh
  Extracts xerces and xalan packages to working directories
  Runs the reconf script on xerces for automake to create configure scripts

script: mkicudir.sh
  Extracts icu packages to working directories

/data/pkgs
/data/pkgs/xalan_c-1.11-docs-html-nograf.tar.gz
/data/pkgs/xerces-c-3.1.4.tar.bz2
/data/pkgs/icu4c-57_1-docs.zip
/data/pkgs/icu4c-52_1-docs.zip
/data/pkgs/xalan_c-1.11-src.tar.gz
/data/pkgs/icu4c-57_1-src.zip
/data/pkgs/icu4c-src-52_1.md5
/data/pkgs/icu4c-57_1-src.tgz
/data/pkgs/icu4c-57_1-data.zip
/data/pkgs/icu4c-src-57_1.md5
/data/pkgs/icu4c-52_1-data.zip

/data/git/XZES40
/data/git/XZES40/CS-Capstone-Project
/data/git/XZES40/Meta-Repo

/data/svn
/data/svn/xalan
/data/svn/xalan/c
/data/svn/xalan/c/trunk  https://svn.apache.org/repos/asf/xalan/c/trunk
/data/svn/xerces
/data/svn/xerces/c
/data/svn/xerces/c/trunk  https://svn.apache.org/repos/asf/xerces/c/trunk

The "mksrcdir.sh" script prepares subdirectories for 
Apache Xerces and Apache Xalan source builds

The "mkicudir.sh" script prepares subdirectories with
extracted ICU Unicode Build Libraries


The scripting tools are found in /data/capstone/tools/*

/data/capstone
/data/capstone/tools
/data/capstone/tools/mkicudir.sh
/data/capstone/tools/mksrcdir.sh
/data/capstone/tools/fixup_permissions.sh

The project documentation notes are found in /data/capstone/docs/*

/data/capstone/docs
/data/capstone/docs/Gtk-Warning.txt
/data/capstone/docs/staff-permissions.txt
/data/capstone/docs/readme-tools.txt
/data/capstone/docs/package-sources.txt
/data/capstone/docs/umask-info.txt

DEVELOPER TARGETS FOR RELEASE BUILDS

/opt
/opt/bin
/opt/etc
/opt/icu
/opt/include
/opt/lib
/opt/pkg
/opt/sbin
/opt/share
/opt/share/man

DEVELOPER TARGETS FOR PRODUCTION BUILDS

/opt/debug
/opt/debug/bin
/opt/debug/etc
/opt/debug/icu
/opt/debug/include
/opt/debug/lib
/opt/debug/pkg
/opt/debug/sbin
/opt/debug/share
/opt/debug/share/man

DEBIAN INSTALLATION FOR 3RD PARTY SOFTWARE

/usr/local
/usr/local/bin
/usr/local/etc
/usr/local/sbin
/usr/local/share
/usr/local/share/fonts
/usr/local/share/xml
/usr/local/share/xml/schema
/usr/local/share/xml/entities
/usr/local/share/xml/misc
/usr/local/share/xml/declaration
/usr/local/share/emacs
/usr/local/share/emacs/24.4
/usr/local/share/emacs/24.4/site-lisp
/usr/local/share/emacs/site-lisp
/usr/local/share/texmf
/usr/local/share/sgml
/usr/local/share/sgml/entities
/usr/local/share/sgml/misc
/usr/local/share/sgml/stylesheet
/usr/local/share/sgml/declaration
/usr/local/share/sgml/dtd
/usr/local/share/ca-certificates
/usr/local/share/man
/usr/local/games
/usr/local/src
/usr/local/lib
/usr/local/lib/python2.7
/usr/local/lib/python2.7/site-packages
/usr/local/lib/python2.7/dist-packages
/usr/local/lib/python3.4
/usr/local/lib/python3.4/dist-packages
/usr/local/include
/usr/local/man



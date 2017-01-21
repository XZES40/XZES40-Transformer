THESE SCRIPTS PREPARE THE SOURCE BUILD ENVIRONMENTS
AND FIXUP PERMISSIONS IN THE SOFTWARE SOURCES TREE

fixup_permissions.sh
  RunAs: root
  Repairs the /data/* permissions of files and directories

mkicudir.sh
  Extracts the Unicode icu4c packages into /data/icu/*
  for clean software builds from sources.

mksrcdir.sh
  Extracts the Xalan and Xerces release packages into /data/src/*
  for clean software builds from sources.

  Extracts the Xalan and Xerces experimental trunk sources into
  /data/src/* for clean software builds from sources.

  Runs the "reconf" script of automake in Xerces in order to
  recreate the standard Makefiles and "configure" script.




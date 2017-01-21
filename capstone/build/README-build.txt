README-build.txt

The "build" directory can go into your $HOME directory.
  It contains the "xerces-build.sh" and "xalan-build.sh" for compiles.
  You may wish to edit these shell script files.  They are
  written to find source products located at: /data/src/...

  There are other scripts that can be used to populate the
  /data/src/... code.  SEE: mkicudir.sh and mksrcdir.sh
  in the "capstone/tools" directory tree.

Directory "build/Xerces-Build" can be used for builds
  Empty the directory for a quick clean to support new build.
  Perform build by:
  cd "$HOME/build/Xerces-Build"
  ../xerces-build.sh

Directory "build/Xalan-Build" can be used for builds
  Empty the directory for a quick clean to support new build.
  Perform build by:
  cd "$HOME/build/Xalan-Build"
  ../xalan-build.sh

There is a dependency order of product builds.  If using the Unicode ICU,
it should be built and installed before you build Xerces and Xalan.

1.  Build Unicode ICU

  Descriptive comments and procedure go here.

  Note: The default builds of Xerces and Xalan do not require ICU.
  There are additional make parameters in order to integrate the ICU
  libraries.

  The actual details will require some deep digging into the Xerces
  and Xalan code.

  The Xerces and Xalan products should be able to perform without
  links to the ICU libraries.  The presence of ICU should dynamically
  unlock the usability of ICU resources.

2.  Build Apache Xerces-C "xerces-build.sh"

  The script creates directory $HOME/build/Xerces-Build/
  and uses this off-of-tree to perform the product builds.

  Debug Build:  "xerces-build.sh Debug"

  Release Build:  "xerces-build.sh"

3.  Build Apache Xalan-C  "xalan-build.sh"

  The script creates directory $HOME/build/Xalan-Build/
  and uses this off-of-tree to perform the product builds.

  Debug Build:  "xalan-build.sh Debug

  Release Build:  "xalan-build.sh"

----------------------------------



xalan-build.sh
Xerces-Build
xerces-build.sh

~/Xalan-Build/
~/Xerces-Build/



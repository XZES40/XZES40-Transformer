README-demo-code.txt  --  Useful items for study to create prototype modules

Prepared by: Steven J. Hathaway

These files are part of my toolkit and available for modification.
You may find these files interesting to study.  If you use these in your project,
the files can be directly licensed to ASF by including the notice artifacts.

None of this code is confidential and has no contractor licensing requirements.

platform.h	recognizes the difference between Windows Studio and GNU tools.
gwinbase.h	declares some WINDOWS data types for GNU compilation toolkits.
dirlist.h	provides a platform independent API to generate keylist lists of files.
keylist.h/c	creates a single-linked list with sublists that are sortable.
btcomp.h/c	creates the default comparitor functions for keylist sorting.
keytable.h/c	creates custom XPATH functions for Xalan-c, uses keylist to implement key/value data tables

exslt.c		creates an XALAN based xml transformer using custom XPath functions.
keytable.h	declares the custom XPath functions.
OspXpathFunction.cpp	implements the XPath functions
OspXpathInstaller.cpp	installs the custom function namespaces and methods into the Xalan engine.


lemslog		is a custom file processor for (Indexed Sequential Access Method) files
		and accommodates incremental reading of the files by message contexts.

The "Projects" directory is used by Windows Visual Studio toolkits for C/C++ software builds.
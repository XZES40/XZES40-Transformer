README - xcvdate - November 27 2007

The Apache libraries are governed by the Apache source code license.
The development effort is not licensed and is given to UNISYS/LEMS
as-is without any encumberances.

 * You may obtain copies of the Apache license at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0


THIS IS NOT PRODUCTION CODE - IT IS PROOF OF CONCEPT ONLY

   Using the Apache XALAN/XERCES libraries for XSL/XML support,
   I have successfully installed a custom XPath function 
   associated with an arbitrary namespace and given an arbitrary
   function name.  I have successfully created an XPath
   function callback class that can use a 'C' library routine
   and repackage the results for the XPath interpreter.

   The test program, once compiled and linked is invoked via:

     xtest dt.xml dt.xsl dt.out

   I have not yet confirmed the deallocation of memory assignment
   resources.

   I have not yet completed the set of exception processing reports.

   The LEMSXpathFunction.hpp and LEMSXpathFunction.cpp are my
   developer notes only and are NOT SOURCE QUALIFIED MATERIAL.

   The xtest source environment and execution environment are
   controlled by setenv.sh and Makefile.

   The source files are:

   LEMSXpath.h                    // the C-API interface
   LEMSXpathDefinitions.hpp       // Handles Windows-GNU compiler issues
   LEMSXpathConvertDate.hpp       // The XPath Function Class, local
   orcvdate.h                     // The 'C' formatting function

   xtest.c                        // The main test program using XalanCAPI.h
   orcvdate.c                     // The 'C' date formatting function
   LEMSXpConvertDate.cpp          // The XPath Interpreter Function - Callback
   LEMSXpathInstaller.cpp         // Installs a local XPath Function

        Test Namespace = http://xml.osp.state.or.us/cjis/lems

        Test Function Name = cvdate

        The XPath Function = static const LEMSXpConvertDate s_cvdateFunction;


THIS IS NOT PRODUCTION CODE - IT IS PROOF OF CONCEPT ONLY

Stylesheet Particulars

 <xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
   xmlns:lx="http://xml.osp.state.or.us/cjis/lems">
 ...
 <xsl:value-of select="lx:cvdate(p1,p2,p3)"/>
 ...
 </xsl:stylesheet>

Function: cvdate()
  p1 = source unformatted date (required)
	This can be any string expression or element reference.
	If this parameter is missing, the test application aborts.

  p2 = formatting code (optional)
	This can be any string expression or element reference. 

  p3 = window year offset from today (optional)
	This can be any value, string expression or element reference.


THIS IS NOT PRODUCTION CODE - IT IS PROOF OF CONCEPT ONLY

I have a date conversion utility (orcvdate.c) that is called
by a XSLT stylesheet XPath callback function (LEMSXpConvertDate.cpp)
and is installed into the XPath interpreter by (LEMSXpathInstaller.cpp)

The main program for testing is (xtest.c) and is compiled and 
linked with the Apache XALAN/XERCES libraries.

I am still troubleshooting the exception processing where
the number of arguments are inconsistent with what is expected.

Samples:

   dt.xml       A sample XML document for testing

   dt.xsl       A sample XSL stylesheet for testing

   dt.out       A sample stylesheet output

There may be some inconsistencies in several files

   Makefile     For one of my development systems.
                GNU/Linux - a Slackware distribution of GLIB
                And links with C/C++ support for main() and
                dynamic library support.

   setenv.sh    For one of my development systems.
                Find XALAN/XERCES dynamic libraries at runtime.


THIS IS NOT PRODUCTION CODE - IT IS PROOF OF CONCEPT ONLY

This is supplied for your LEMS development future support of XML.

Sincerely,
Steven J. Hathaway
Oregon State Police
Salem, Oregon

November 27 2007

Email: <steven.j.hathaway@state.or.us>
Phone: 503-378-3055 x 55010



/*
 * Oregon State Police, Law Enforcement Data System
 * Salem, Oregon
 *
 * Copyright 2009 Oregon State Police / LEDS
 *
 * Using Apache Xalan/Xerces libraries that are licensed
 * under the Apache License, Version 2.0 (the "License").
 *
 * You may obtain copies of the Apache license at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Since this software is a custom extension to the Xalan
 * XPATH supported library, and no change is made to the
 * distributed Xalan/Xerces sources,  this software is made
 * available for incorporating into any product.
 *
 * The software contained herein is distributed on an "AS IS"
 * basis without warranties or conditions of any kind, either
 * express or implied.
 */

/* Main Program for 'C' Language based XSLT Transformation with custom
 * XPath extension cvdate function.
 */

#include <windows.h>

#include <stdio.h>

#include <xalanc/XalanTransformer/XalanCAPI.h>

/* Declare the Local LEMS Xpath Functions Installer */
 
#include "OspXpath.h"


/* Define ISO C functions for deprecated POSIX functions */
#define strdup _strdup

XalanHandle	xalan;
XalanCSSHandle	theCSSHandle;
XalanPSHandle	thePSHandle;

int main (int argc, char * argv[])
{
	int     args;
	char * theXMLFileName;
	char * theXSLFileName;
	char * theOutFileName;
	int	rtn;
	char * err;

	args = argc;

	if (args > 1)
	{
		theXMLFileName = strdup(argv[1]);
	}
	else
	{
		printf("\nERROR - XML Filename Not Specified\n");
		exit(1);
	}

	if (args > 2)
	{
		theXSLFileName = strdup(argv[2]);
	}

	if (args > 3)
	{
		theOutFileName = strdup(argv[3]);
	}

	XalanInitialize();

	xalan = CreateXalanTransformer();

	if (!xalan)
	{
		printf("\nERROR - Unable to create Xalan Transformer\n");
		exit(1);
	}
	else
	{
		printf("\nXalan Transformer Created - Success\n");
	}

/*
 * Install the local LEMS Xpath Functions into XALAN xslt processor.
*/

	OSPXpathInstall(xalan,global);

	rtn = XalanParseSource( theXMLFileName, xalan, &thePSHandle );

	if (rtn)
	{
		printf("\nERROR - Unable to parse XML File: %s\n", theXMLFileName);
		err = (char *) XalanGetLastError( xalan );
		printf("%s\n", err);
		exit(1);
	}
	else
	{
		printf("\nXalan Parse Source - XML Read Success\n");
	}

	if (args > 2)
	{
		rtn = XalanCompileStylesheet( theXSLFileName, xalan, &theCSSHandle );

		if (rtn)
		{
			printf("\nERROR - Unable to Compile XSL Stylesheet: %s\n", theXSLFileName);
			err = (char *) XalanGetLastError( xalan );
			printf("%s\n", err);
			exit(1);
		}
		else
		{
			printf("\nXalan Compile Stylesheet - Success\n");
		}
	}

	if (args > 3)
	{
		rtn = XalanTransformToFilePrebuilt( thePSHandle, theCSSHandle, theOutFileName, xalan);

		if (rtn)
		{
			printf("\nERROR - Unable To Translate XML using XSLT\n");
			err = (char *) XalanGetLastError( xalan );
			printf("%s\n", err);
			exit(1);
		}
		else
		{
			printf("\nXalan Transformation Success - To File: %s\n", theOutFileName);
		}
	}

/* Prepare to Exit */

	if (args > 2)
		XalanDestroyCompiledStylesheet( theCSSHandle, xalan );

	XalanDestroyParsedSource( thePSHandle, xalan );

/* Uninstall LEMS Local Functions */
	OSPXpathUninstall(xalan,global);

	DeleteXalanTransformer(xalan);

	exit(0);
} // main()


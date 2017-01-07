///////////////////////////////////////////////////////////////////////////////
// Filename: xzes40.cpp
// Description: Main entrypoint for the XZES40 Transformer application
///////////////////////////////////////////////////////////////////////////////

#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include <xalanc/XalanTransformer/XalanTransformer.hpp>

#include <cstdlib>
#include <iostream>

#include <lib.hpp>
#include <transform.hpp>

XALAN_USING_XERCES(XMLPlatformUtils)
XALAN_USING_XALAN(XalanTransformer)
XALAN_USING_XALAN(XSLTInputSource)
XALAN_USING_XALAN(XSLTResultTarget)

std::string USAGE = "\
Usage: \n\
  a.out --xml=input.xml --xsl=style.xslt [--out=output file] \n\
";

// ----------------------------------------------------------------------------
// int main ( int argc , char* argv[] )
//
// Performs the following operations:
// 1. Parses user input (xml, xslt, and output file locations).
// 2. Passes cli arguments to 'transform' function. 
// 3. Exits with an appropriate status code.
// ----------------------------------------------------------------------------
int main( int argc , char * argv[] )
{
	int status;

    // Parse CLI arguments into struct `cli_arguments_t`
    cli_arguments_t* args = parse_args( &argc , &argv );

	// User supplied bad arguments or input files that don't exist.
    if( args->xml == "\0" || args->xsl == "\0" ||
        args->xml == ""   || args->xsl == "" )
	{
		// Print usage
        std::cout << USAGE;
		// Set exit status code
		status = EXIT_FAILURE;
	}
	else
	{
		// Pass args to transform
    	status = transform( args );
	}

    return status;
}

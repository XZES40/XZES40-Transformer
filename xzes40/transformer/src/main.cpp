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
    xzes::cli_arguments_t* args = xzes::parse_args( &argc , &argv );

	// User input files that do not exist.
    if( args->xml == "\0" || args->xsl == "\0" ||
	// User supplied empty xml or xsl arguments.
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
    	status = xzes::transform_documents( args );
	}

    return status;
}

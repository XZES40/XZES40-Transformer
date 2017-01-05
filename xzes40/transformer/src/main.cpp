///////////////////////////////////////////////////////////////////////////////
// Filename: xzes40.cpp
// Description: Main entrypoint for the XZES40 Transformer application
///////////////////////////////////////////////////////////////////////////////

#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include <xalanc/XalanTransformer/XalanTransformer.hpp>

#include <iostream>

#include <lib.hpp>
#include <transform.hpp>

XALAN_USING_XERCES(XMLPlatformUtils)
XALAN_USING_XALAN(XalanTransformer)
XALAN_USING_XALAN(XSLTInputSource)
XALAN_USING_XALAN(XSLTResultTarget)

// ----------------------------------------------------------------------------
// int main ( int argc , char* argv[] )
// Performs the following operations:
// 1. Parses user input (xml, xslt, and output file locations).
// 2. Passes cli arguments to 'transform' function. 
// 3. Exits with an appropriate status code.
// ----------------------------------------------------------------------------
int main( int argc , char * argv[] )
{
    // Parse CLI arguments into struct `cli_args`

    // Pass file to transform

    int status = EXIT_SUCCESS;

    return status;
}

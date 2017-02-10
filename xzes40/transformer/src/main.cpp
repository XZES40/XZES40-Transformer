///////////////////////////////////////////////////////////////////////////////
// Copyright 2017, the Oregon State University Capstone Project 'XZES40'
// with the Apache Foundation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Filename: xzes40.cpp
// Description: Main entrypoint for the XZES40 Transformer application
///////////////////////////////////////////////////////////////////////////////

// #include <xalanc/Include/PlatformDefinitions.hpp>
// #include <xercesc/util/PlatformUtils.hpp>

// #include <xalanc/XalanTransformer/XalanTransformer.hpp>

#include <cstdlib>
#include <iostream>

#include <lib.hpp>
#include <transform.hpp>

// XALAN_USING_XERCES(XMLPlatformUtils);
// XALAN_USING_XALAN(XalanTransformer);
// XALAN_USING_XALAN(XSLTInputSource);
// XALAN_USING_XALAN(XSLTResultTarget);

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
    if( args->xml.uri == "\0" || args->xsl.uri == "\0" ||
	// User supplied empty xml or xsl arguments.
        args->xml.uri == ""   || args->xsl.uri == "" )
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

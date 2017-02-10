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
// Filename: transform.cpp
// Description: Definition and documentation for custom structs, functions,
// and classes used in XZES40 Transformer transformation pipeline.
///////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <iostream>

#include <cache.hpp>
#include <document.hpp>
#include <lib.hpp>
#include <transform.hpp>

//define namespace
XALAN_USING_XERCES(XMLPlatformUtils);
XALAN_USING_XALAN(XalanTransformer);
XALAN_USING_XALAN(XSLTInputSource);
XALAN_USING_XALAN(XSLTResultTarget);

// ----------------------------------------------------------------------------
// int transform_documents( xzes::cli_arguments* args )
//
// Performs the following operations:
// 1. Fetches input files from cache
// 2. Calls 'performs transform' with parsed input xml and xslt files.
// 3. Returns an appropriate status code.
// ----------------------------------------------------------------------------
int xzes::transform_documents( xzes::cli_arguments_t* args )
{
    int status = EXIT_SUCCESS;

    // TODO: Remove debugging information
    std::cout << "Processing xml file " << args->xml.uri
              << " xsl file "           << args->xsl.uri
              << ". Writing to "        << args->out.uri
              << "."                    << std::endl;

    // Create transformation object;
    xzes::Transformer t = xzes::Transformer( );
    // Initialized xml and xls document
    t.pass_xml( args->xml.uri );
    t.pass_xsl( args->xsl.uri );
    // Set the output path
    t.set_output( args->out.uri );
    // Start transformation
    t.transformation( t );

    // TODO cache output object

    return status;
}

// ----------------------------------------------------------------------------
// Transformer initialize
//
// Initialized a document Transformer.
// ----------------------------------------------------------------------------
xzes::Transformer::Transformer( )
{
	// Initialize xalan
	XMLPlatformUtils::Initialize();
	XalanTransformer::initialize();
	
}

// ----------------------------------------------------------------------------
// xzes::Document transformation( )
//
// Transforms two Document structs into a new Document.
// ----------------------------------------------------------------------------
xzes::Document xzes::Transformer::transformation( xzes::Transformer doc_source )
{
    int status = EXIT_SUCCESS;
    
    xzes::dom_t parsedSource = doc_source.xml.get_contents
    xzes::dom_t stylesheet   = doc_source.xls.get_contents
    string output            = doc_source.out
    
    //start transform
    xalan.transform(parsedSource, stylesheet, output);
    
    
    //Terminate xalan
    XalanTransformer::terminate();
    XMLPlatformUtils::Terminate();
    XalanTransformer::ICUCleanUp();
    
    return status;
}

// ----------------------------------------------------------------------------
// xzes::Document pass_xml( string xml )
// Pass the xml uri to document class
// ----------------------------------------------------------------------------
xzes::Document xzes::Transformer::pass_xml( xzes::uri_t xml_path )
{
    // pass the xml uri
    // 1 means this is xml file
    // After we pass uri, this xml actually have been initialized
    xml = xzes::Document(xml_path,1);
    return xml;
}


// ----------------------------------------------------------------------------
// Document pass_xsl( string xsl )
// Pass the xml uri to document class
// ----------------------------------------------------------------------------
xzes::Document xzes::Transformer::pass_xsl( xzes::uri_t xsl_path )
{
    // pass the xsl uri
    // 2 means this is xls file
    // After we pass uri, this xml actually have been initialized
    xsl = xzes::Document(xsl_path,2);
    return xsl;
}

// ----------------------------------------------------------------------------
// set_output( string out )
// Set the output of the transformation( ) method.
// ----------------------------------------------------------------------------
int xzes::Transformer::set_output( xzes::uri_t out_path )
{
    // set the out uri;
    out = out_path;
    
    // return status for debug
    int status = EXIT_SUCCESS;
    return status;
}

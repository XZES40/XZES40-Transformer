///////////////////////////////////////////////////////////////////////////////
// Filename: transform.cpp
// Description: Definition and documentation for custom structs, functions,
// and classes used in XZES40 Transformer transformation pipeline.
///////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <iostream>

#include <cache.hpp>
#include <lib.hpp>
#include <transform.hpp>

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
    // TODO: Remove debugging information
    std::cout << "Processing xml file " << args->xml
              << " xsl file "           << args->xsl
              << ". Writing to "        << args->out
              << "."                    << std::endl;

    // Create transformation object;
    xzes::Transformer t;
    t.parse_xml( &args->xml );
    t.parse_xsl( &args->xsl );
    t.set_output( &args->out );
    t.transformation( );

    return EXIT_SUCCESS;
}

// ----------------------------------------------------------------------------
// xzes::document_t transformation( )
//
// Transforms two document_t structs into a new document_t.
// ----------------------------------------------------------------------------
xzes::document_t* xzes::Transformer::transformation( )
{
    // TODO Write this
    xzes::document_t* object = new xzes::document_t;
    return object;
}

// ----------------------------------------------------------------------------
// xzes::document_t parse_xml( string* xml )
//
// Parses an XML file into a document_t.
// ----------------------------------------------------------------------------
xzes::document_t* xzes::Transformer::parse_xml( std::string* xml )
{
    // TODO Write this
    xzes::document_t* object = new xzes::document_t;
    return object;
}

// ----------------------------------------------------------------------------
// document_t parse_xsl( string* xsl )
//
// Parses an XSLT file into a document_t.
// ----------------------------------------------------------------------------
xzes::document_t* xzes::Transformer::parse_xsl( std::string* xsl )
{
    // TODO Write this
    xzes::document_t* object = new xzes::document_t;
    return object;
}

// ----------------------------------------------------------------------------
// set_output( string* out )
//
// Set the output of the transformation( ) method.
// ----------------------------------------------------------------------------
int xzes::Transformer::set_output( std::string* out )
{
    // TODO Write this
    int status = EXIT_SUCCESS;
    return status;
}

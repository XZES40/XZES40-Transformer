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
    t.parse_xml( args->xml );
    t.parse_xsl( args->xsl );
    t.set_output( args->out );
    t.transformation( );

    // TODO cache output object.

    return status;
}

// ----------------------------------------------------------------------------
// Transformer initialize
//
// Initialized a document Transformer.
// ----------------------------------------------------------------------------
xzes::Transformer::Transformer( )
{
    // TODO write this method
    xml = Document( );
    xsl = Document( );
    out = Document( );
}

// ----------------------------------------------------------------------------
// xzes::Document transformation( )
//
// Transforms two Document structs into a new Document.
// ----------------------------------------------------------------------------
xzes::Document xzes::Transformer::transformation( )
{
    // TODO Write this
    return xml;
}

// ----------------------------------------------------------------------------
// xzes::Document parse_xml( string xml )
//
// Parses an XML file into a Document.
// ----------------------------------------------------------------------------
xzes::Document xzes::Transformer::parse_xml( xzes::uri_t xml_path )
{
    // Check cache for document
    xml = xzes::Document( );
    return xml;
}

// ----------------------------------------------------------------------------
// Document parse_xsl( string xsl )
//
// Parses an XSLT file into a Document.
// ----------------------------------------------------------------------------
xzes::Document xzes::Transformer::parse_xsl( xzes::uri_t xsl_path )
{
    // TODO Write this
    xsl = xzes::Document( );
    return xsl;
}

// ----------------------------------------------------------------------------
// set_output( string out )
//
// Set the output of the transformation( ) method.
// ----------------------------------------------------------------------------
int xzes::Transformer::set_output( xzes::uri_t out_path )
{
    // TODO Write this
    int status = EXIT_SUCCESS;
    return status;
}

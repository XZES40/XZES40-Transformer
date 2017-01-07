///////////////////////////////////////////////////////////////////////////////
// Filename: transform.cpp
// Description: Definition and documentation for custom structs, functions,
// and classes used in XZES40 Transformer transformation pipeline.
///////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <iostream>

#include <transform.hpp>
#include <lib.hpp>

// ----------------------------------------------------------------------------
// int transform( cli_arguments args )
//
// Performs the following operations:
// 1. Fetches input files from cache
// 2. Calls 'performs transform' with parsed input xml and xslt files.
// 3. Returns an appropriate status code.
// ----------------------------------------------------------------------------
int transform( cli_arguments_t* args )
{
    // TODO: Remove debugging information
    std::cout << "Processing xml file " << args->xml
              << " xsl file "           << args->xsl
              << ". Writing to "        << args->out
              << "."                    << std::endl;

    // Create transformation object;
    t = Transformer;
    t.parse_xml( args->xml );
    t.parse_xsl( args->xsl );
    t.transform( );

    return EXIT_SUCCESS;
}

// ----------------------------------------------------------------------------
// class Transformer
//
// May perform the following operations:
// - Parse an xml file into a document_t.
// - Parse an xsl file into a document_t.
// - Transform the xml and xsl document_t into the out document_t.
// ----------------------------------------------------------------------------
class Transformer
{
        document_t xml, xsl, out;

    public:
        // --------------------------------------------------------------------
        // document_t transform( document_t xml, document_t xsl )
        //
        // Transforms two document_t structs into a new document_t.
        // --------------------------------------------------------------------
        document_t transform( document_t xml, document_t xsl )
        {
            // TODO Write this
            document_t object;
            return object;
        }

        // --------------------------------------------------------------------
        // document_t parse_xml( string xml )
        //
        // Parses an XML file into a document_t.
        // --------------------------------------------------------------------
        document_t parse_xml( std::string xml )
        {
            // TODO Write this
            document_t object;
            return object;
        }

        // --------------------------------------------------------------------
        // document_t parse_xsl( string xsl )
        //
        // Parses an XSLT file into a document_t.
        // --------------------------------------------------------------------
        document_t parse_xsl( std::string xsl )
        {
            // TODO Write this
            document_t object;
            return object;
        }
};

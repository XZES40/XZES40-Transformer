///////////////////////////////////////////////////////////////////////////////
// Filename: transform.hpp
// Description: Declaration of custom structs, functions, and classes used in
// XZES40 Transformer transformation pipeline.
///////////////////////////////////////////////////////////////////////////////

#include <string>

#include <lib.hpp>
#include <cache.hpp>

#if !defined transform
#define transform

int transform( cli_arguments_t );

class Transformer
{
    private:
        document_t xml, xsl, out;

    public:
        struct document_t transform( document_t, document_t );
        struct document_t parse_xml( std::string );
        struct document_t parse_xsl( std::string );
};

#endif

///////////////////////////////////////////////////////////////////////////////
// Filename: transform.hpp
// Description: Declaration of custom structs, functions, and classes used in
// XZES40 Transformer transformation pipeline.
///////////////////////////////////////////////////////////////////////////////

#include <string>

#include <lib.hpp>

#if !defined transform
#define transform

namespace xzes
{
    // ------------------------------------------------------------------------
    // class Transformer
    //
    // May perform the following operations:
    // - Parse an xml file into a document_t.
    // - Parse an xsl file into a document_t.
    // - Transform the xml and xsl document_t into the out document_t.
    // ------------------------------------------------------------------------
    class Transformer
    {
        private:
//            xzes::document_t xml, xsl, out;

        public:
            xzes::document_t* transformation( );
            xzes::document_t* parse_xml( std::string* );
            xzes::document_t* parse_xsl( std::string* );
            int set_output( std::string* out );
    };

    int transform_documents( cli_arguments_t* );
}

#endif

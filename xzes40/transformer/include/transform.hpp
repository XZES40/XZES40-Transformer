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
    // - Parse an xml file into a Document.
    // - Parse an xsl file into a Document.
    // - Transform the xml and xsl Document into the out Document.
    // ------------------------------------------------------------------------
    class Transformer
    {
        private:
//            xzes::Document xml, xsl, out;

        public:
            xzes::Document* transformation( );
            xzes::Document* parse_xml( std::string* );
            xzes::Document* parse_xsl( std::string* );
            int set_output( std::string* out );
    };

    int transform_documents( cli_arguments_t* );
}

#endif

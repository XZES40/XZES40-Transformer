///////////////////////////////////////////////////////////////////////////////
// Filename: transform.hpp
// Description: Declaration of custom structs, functions, and classes used in
// XZES40 Transformer document handling pipeline.
///////////////////////////////////////////////////////////////////////////////

#include <lib.hpp>

#if !defined document
#define document

namespace xzes
{
    class Document
    {
        private:
            dom_t contents;
            id_t  uid;
            uri_t uri;

        public:
            Document( );
            int set_uri( uri_t );
            uri_t get_uri( );
            int set_contents( dom_t );
            dom_t get_contents( );
            int set_id( id_t );
            id_t get_id( );
    };
}

#endif

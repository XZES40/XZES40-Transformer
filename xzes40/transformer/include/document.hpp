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
            int contents;
            hash_t id;
            std::string uri;

        public:
            Document( );
            int set_uri( std::string* );
            std::string get_uri( );
            int set_contents( );
            int get_contents( );
            int set_id( );
            int get_id( );
    };

}

#endif

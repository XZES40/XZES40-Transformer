///////////////////////////////////////////////////////////////////////////////
// Filename: cache.hpp
// Description: Declaration of custom structs, functions, and classes used in
// XZES40 Transformer cacheing pipeline.
///////////////////////////////////////////////////////////////////////////////

#include <lib.hpp>


#if !defined cache
#define cache

namespace xzes
{
    class Cache {
        private:

        public:
            document_t* get( document_t* );
            document_t* set( document_t* );
            int del( document_t* );
    };
}

#endif

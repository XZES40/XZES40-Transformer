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
            Document* get( Document* );
            Document* set( Document* );
            int del( Document* );
    };
}

#endif

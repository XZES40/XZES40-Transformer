///////////////////////////////////////////////////////////////////////////////
// Filename: cache.hpp
// Description: Declaration of custom structs, functions, and classes used in
// XZES40 Transformer cacheing pipeline.
//
// All class definitions are documented in src/cache.cpp
///////////////////////////////////////////////////////////////////////////////

#include <lib.hpp>
#include <document.hpp>


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

///////////////////////////////////////////////////////////////////////////////
// Filename: cache.cpp
// Description: Definition and documentation for custom functions,
// and classes used in XZES40 Transformer cacheing pipeline.
///////////////////////////////////////////////////////////////////////////////

#include <cstdlib>

#include <cache.hpp>
#include <lib.hpp>

// --------------------------------------------------------------------
// Document* Cache::get( Document* )
//
// Attempts to get a Document from the cache.
// Returns the object if it is in the cache.
// Returns NULL if it does not exist in the cache.
// --------------------------------------------------------------------
xzes::Document* xzes::Cache::get( xzes::Document* object )
{
    // TODO: Write this function
    return object;
}

// --------------------------------------------------------------------
// Document* Cache::set( Document* )
//
// Attempts to put a Document object in the in-memory cache.
// Returns the object if successful.
// Returns NULL if not successful.
// --------------------------------------------------------------------
xzes::Document* xzes::Cache::set( xzes::Document* object )
{
    // TODO: Write this function
    return object;
}

// --------------------------------------------------------------------
// int Cache::del( Document* )
//
// Deletes an object from the cache.
// Returns EXIT_SUCCESS if the object was delete successfully.
// Returns EXIT_SUCCESS if the object was not in the cache (still does
// not exist in cache).
// Returns EXIT_FAILURE if the object is in the cache but was not
// deleted.
// --------------------------------------------------------------------
int xzes::Cache::del( xzes::Document* object )
{
    // TODO: Write ths function
    int status = EXIT_SUCCESS;
    return status;
}

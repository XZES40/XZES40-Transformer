///////////////////////////////////////////////////////////////////////////////
// Filename: cache.cpp
// Description: Definition and documentation for custom structs, functions,
// and classes used in XZES40 Transformer cacheing pipeline.
///////////////////////////////////////////////////////////////////////////////

#include <cstdlib>

#include <cache.hpp>
#include <lib.hpp>

// ----------------------------------------------------------------------------
// document_t* Cache::get( document_t* )
//
// Attempts to get a document_t from the cache.
// Returns the object if it is in the cache.
// Returns NULL if it does not exist in the cache.
// ----------------------------------------------------------------------------
xzes::document_t* xzes::Cache::get( xzes::document_t* )
{
    // TODO: Write this function
    xzes::document_t* object = new xzes::document_t;
    return object;
}

// ----------------------------------------------------------------------------
// document_t* Cache::set( document_t* )
//
// Attempts to put a document_t object in the in-memory cache.
// Returns the object if successful.
// Returns NULL if not successful.
// ----------------------------------------------------------------------------
xzes::document_t* xzes::Cache::set( xzes::document_t* )
{
    // TODO: Write this function
    xzes::document_t* object = new xzes::document_t;
    return object;
}

// ----------------------------------------------------------------------------
// int Cache::del( document_t* )
//
// Deletes an object from the cache.
// Returns EXIT_SUCCESS if the object was delete successfully.
// Returns EXIT_SUCCESS if the object was not in the cache (still does not exist in cache).
// Returns EXIT_FAILURE if the object is in the cache but was not deleted.
// ----------------------------------------------------------------------------
int xzes::Cache::del( xzes::document_t* )
{
    // TODO: Write ths function
    int status = EXIT_SUCCESS;
    return status;
}

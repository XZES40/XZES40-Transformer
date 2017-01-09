///////////////////////////////////////////////////////////////////////////////
// Filename: transform.cpp
// Description: Definition and documentation for custom structs, functions,
// and classes used in XZES40 Document handling pipeline.
///////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <string>

#include <lib.hpp>
#include <document.hpp>

// ----------------------------------------------------------------------------
// class Document
// 
// Stores all data about documents and functions to act on that data.
// Data includes:
// - Document contents (Parsed XML/XSL document, not raw)
// - Document ID  (Unique identifier for document)
// - Document URI (Path to find file)
// Functions include:
// - set_id( )
// - set_uri( )
// - set_uri( ) 
//
// **NOTE** The majority of the Caching happens in this Document class.
// The set_contents( ) method uses the Cache to store and reference the Xerces
// object.
// ----------------------------------------------------------------------------
// int Document( std::string file_path);
//
// Intializes a document by setting the uri, path, and contents.
// ----------------------------------------------------------------------------
xzes::Document::Document( )
{
    // TODO: Write this method
}

// ----------------------------------------------------------------------------
// int set_uri( );
// ----------------------------------------------------------------------------
int xzes::Document::set_uri( std::string* file_path )
{
    // TODO: Write this method
    int status = EXIT_SUCCESS;
    return status;

}

// ----------------------------------------------------------------------------
// int get_uri( );
// ----------------------------------------------------------------------------
std::string xzes::Document::get_uri( )
{
    // TODO: Write this method
    return uri;
}

// ----------------------------------------------------------------------------
// int set_id( );
// ----------------------------------------------------------------------------
int xzes::Document::set_id( )
{
    // TODO: Write this method
    int status = EXIT_SUCCESS;
    return status;

}

// ----------------------------------------------------------------------------
// int get_id( );
// ----------------------------------------------------------------------------
int xzes::Document::get_id( )
{
    // TODO: Write this method
    int status = EXIT_SUCCESS;
    return status;
}

// ----------------------------------------------------------------------------
// int set_contents( );
// ----------------------------------------------------------------------------
int xzes::Document::set_contents( )
{
    // TODO: Write this method
    int status = EXIT_SUCCESS;
    return status;

}

// ----------------------------------------------------------------------------
// int get_contents( );
// ----------------------------------------------------------------------------
int xzes::Document::get_contents( )
{
    // TODO: Write this method
    int status = EXIT_SUCCESS;
    return status;
}

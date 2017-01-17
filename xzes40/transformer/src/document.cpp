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
int xzes::Document::set_uri( xzes::uri_t file_path )
{
    // TODO: Write this method
    int status = EXIT_SUCCESS;
    return status;

}

// ----------------------------------------------------------------------------
// int get_uri( );
//
// Get the Universal Resource Identifier of a Document.
// E.g., file:///home/username/docs/example.xml
// or https:///www.example.com/docs/example.xml
// ----------------------------------------------------------------------------
xzes::uri_t xzes::Document::get_uri( )
{
    // TODO: Write this method
    xzes::uri_t file_path;
    return file_path;
}

// ----------------------------------------------------------------------------
// int set_id( );
//
// Set the Unique ID of a Document.
// ----------------------------------------------------------------------------
int xzes::Document::set_id( xzes::id_t id )
{
    // TODO: Write this method
    int status = EXIT_SUCCESS;
    return status;

}

// ----------------------------------------------------------------------------
// id_t get_id( );
//
// Return the unique ID of a Document.
// ----------------------------------------------------------------------------
xzes::id_t xzes::Document::get_id( )
{
    // TODO: Write this method
    xzes::id_t obj;
    return obj;
}

// ----------------------------------------------------------------------------
// int set_contents( );
//
// Set the DOM contents of a Document.
// ----------------------------------------------------------------------------
int xzes::Document::set_contents( xzes::dom_t contents )
{
    // TODO: Write this method
    int status = EXIT_SUCCESS;
    return status;

}

// ----------------------------------------------------------------------------
// int get_contents( );
//
// Return the DOM contents of a Document.
// ----------------------------------------------------------------------------
xzes::dom_t xzes::Document::get_contents( )
{
    // TODO: Write this method
    xzes::dom_t doc;
    return doc;
}

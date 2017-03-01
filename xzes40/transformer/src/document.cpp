///////////////////////////////////////////////////////////////////////////////
// Copyright 2017, the Oregon State University Capstone Project 'XZES40'
// with the Apache Software Foundation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Filename: transform.cpp
// Description: Definition and documentation for custom structs, functions,
// and classes used in XZES40 Document handling pipeline.
///////////////////////////////////////////////////////////////////////////////

#include <document.hpp>

XALAN_USING_XALAN(XSLTInputSource);

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
// - set_content( )
//
// **NOTE** The majority of the Caching happens in this Document class.
// The set_content( ) method uses the Cache to store and reference the Xerces
// object.
// ----------------------------------------------------------------------------
// int Document( std::string file_path);
//
// Intializes a document by setting the uri, path, and contents.
// ----------------------------------------------------------------------------
xzes::Document::Document( xzes::uri_t file_path, Cache *storeList)
{
    //Cache::Cache storeList;

    // Set the file path to uri
    set_uri( file_path );

    // Set the Document ID
    set_id( );

    // Compile the document now that you know the type
    if(!storeList->search(uid)){
        compile();
        storeList->set( uid , doc , uri );
    } else {
        set_content( storeList->get( uid ));
    }
}

// ----------------------------------------------------------------------------
// Empty Document constructor
// ----------------------------------------------------------------------------
xzes::Document::Document( )
{
    xzes::uri_t blank_uri;
    blank_uri.uri = "";

    set_uri( blank_uri );
    set_id( );
}

// ----------------------------------------------------------------------------
// int set_uri( );
// ----------------------------------------------------------------------------
int xzes::Document::set_uri( xzes::uri_t file_path )
{
    uri = file_path ;

    //return status for debug
    int status = SUCCESS;
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
    return uri;
}

// ----------------------------------------------------------------------------
// int set_id( );
//
// Set the Unique ID of a Document.
// ----------------------------------------------------------------------------
int xzes::Document::set_id( )
{
    // Set the uid to the output of hash_uri
    uid = _hash_uri( );

    //return status for debug
    int status = SUCCESS;
    return status;
}

// ----------------------------------------------------------------------------
// id_t get_id( );
//
// Return the unique ID of a Document.
// ----------------------------------------------------------------------------
xzes::id_t xzes::Document::get_id( )
{
    xzes::id_t obj = uid;
    return obj;
}

// ----------------------------------------------------------------------------
// int set_content( );
//
// Set the DOM contents of a Document.
// ----------------------------------------------------------------------------
int xzes::Document::set_content( xzes::doc_t* content )
{
    //return status for debug
    int status = SUCCESS;

    doc = content;

    return status;
}

// ----------------------------------------------------------------------------
// doc_t get_content( );
//
// Get the DOM contents of a Document.
// ----------------------------------------------------------------------------
xzes::doc_t* xzes::Document::get_content( )
{
    return doc;
}

// ----------------------------------------------------------------------------
// int compile( );
//
// Compile document.
// ----------------------------------------------------------------------------
int xzes::Document::compile( )
{
    // return status for error handling
    int status = SUCCESS;
    
    doc_t* output_document = new xzes::doc_t;
    output_document->obj   = new XSLTInputSource( uri.uri.c_str() );

    // store the paresed file to class
    set_content( output_document );

    return status;
}

// ----------------------------------------------------------------------------
// id_t _hash_uri( )
//
// Hashes the uri member of the class into an id_t
// ----------------------------------------------------------------------------
xzes::id_t xzes::Document::_hash_uri( )
{
    id_t output_id;

    output_id.id = xzes::_hash(uri.uri.c_str());

    return output_id;
}



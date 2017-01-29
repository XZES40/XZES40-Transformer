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

#include <cstdlib>
#include <string>
#include <functional>
#include <iostream>

#include <lib.hpp>
#include <document.hpp>

// Xerces and Xalan
#include <xalanc/XalanTransformer/XalanTransformer.hpp>
XALAN_USING_XALAN(  XalanTransformer  );
XALAN_USING_XERCES( XMLPlatformUtils  );

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
xzes::Document::Document( xzes::uri_t file_path , int type)
{
    // Set the file path to uri
    set_uri( file_path );

    // Set the Document ID
    set_id( );

    // Set the document flag typ
    set_type( type );

    // Compile the document now that you know the type
    compile( );
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
    set_type( XML_DOC_TYPE );
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
// int set_xml_content( );
//
// Set the DOM contents of a Document.
// ----------------------------------------------------------------------------
int xzes::Document::set_xml_content( xzes::dom_t content )
{
    //return status for debug
    int status = SUCCESS;

    xml = content;

    return status;
}

// ----------------------------------------------------------------------------
// int set_xsl_content( );
//
// Set the DOM contents of a Document.
// ----------------------------------------------------------------------------
int xzes::Document::set_xsl_content( xzes::style_t content )
{
    //return status for debug
    int status = SUCCESS;

    xsl = content;

    return status;
}

// ----------------------------------------------------------------------------
// xzes::dom_t get_xml_content( );
//
// Return the DOM contents of a Document.
// ----------------------------------------------------------------------------
xzes::dom_t xzes::Document::get_xml_content( )
{
    return xml;
}

// ----------------------------------------------------------------------------
// xzes::style_t get_xsl_content( );
//
// Return the Stylesheet contents of a Document.
// ----------------------------------------------------------------------------
xzes::style_t xzes::Document::get_xsl_content( )
{
    return xsl;
}


// ----------------------------------------------------------------------------
// int compile( );
//
// Compile either xml or xsl document depending ont he doc_type.
// ----------------------------------------------------------------------------
int xzes::Document::compile( )
{
    int status;

    if (doc_type == XML_DOC_TYPE)
        status = _compile_xml( );
    else if (doc_type == XSL_DOC_TYPE)
        status = _compile_xsl( );
    else
        status = FAILURE;

    return status;
}

// ----------------------------------------------------------------------------
// int _compile_xml( );
//
// parse the xml file to dom document, and then store it back to document class
// ----------------------------------------------------------------------------
int xzes::Document::_compile_xml( )
{
    // return status for error handling
    int status = SUCCESS;

    //Initialize function
    XMLPlatformUtils::Initialize();
    XalanTransformer::initialize();

    //create a xalantransformer
    XalanTransformer the_xalan_transformer;

    // Create the dom_t object
    dom_t output_xml_doc;

    the_xalan_transformer.parseSource( uri.uri.c_str() , output_xml_doc.obj );

    // store the paresed file to class
    set_xml_content( output_xml_doc );

    //Terminate xalan
    XalanTransformer::terminate( );
    XMLPlatformUtils::Terminate( );
    XalanTransformer::ICUCleanUp( );

    return status;
}

// ----------------------------------------------------------------------------
// int _compile_xsl( );
//
// compile the stylesheet for future reuse, and store it back to document class
// ----------------------------------------------------------------------------
int xzes::Document::_compile_xsl( )
{
    int status = SUCCESS;

    // Initialize the Xalan Transformer
    XalanTransformer the_xalan_transformer;

    xzes::style_t output_stylesheet;

    //Using xalan to compile stylesheet to binary file
    the_xalan_transformer.compileStylesheet( uri.uri.c_str(), output_stylesheet.obj );

    //after compiled stylesheet, store it in our class
    Document::set_xsl_content( output_stylesheet );

    //Terminate xalan
    XalanTransformer::terminate( );
    XMLPlatformUtils::Terminate( );
    XalanTransformer::ICUCleanUp( );

    //return status for debug
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
    output_id.id = xzes::_hash( uri.uri );

    return output_id;
}

// ----------------------------------------------------------------------------
// int set_type( int )
//
// Sets the document type.
// ----------------------------------------------------------------------------
int xzes::Document::set_type( int t )
{
    int status = SUCCESS;

    doc_type = t;

    return status;
}

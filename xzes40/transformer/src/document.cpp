///////////////////////////////////////////////////////////////////////////////
// Copyright 2017, the Oregon State University Capstone Project 'XZES40'
// with the Apache Foundation
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
// - set_contents( )
//
// **NOTE** The majority of the Caching happens in this Document class.
// The set_contents( ) method uses the Cache to store and reference the Xerces
// object.
// ----------------------------------------------------------------------------
// int Document( std::string file_path);
//
// Intializes a document by setting the uri, path, and contents.
// ----------------------------------------------------------------------------
xzes::Document::Document( xzes::uri_t file_path , int doc_type)
{
    // set the file path to uri
    set_uri(file_path);
    // hash the file path
    std:: string s = file_path;
    std::hash<std::string> hash_fn;
    size_t hash = hash_fn(s);
    // and set the id
    set_id(hash);
    
    // we need konw which document it is
    // 1 means this is xml file, parse it and set the contents
    if(doc_type == 1)
    {
        parse_xml(file_path);
    }
    // 2 means this is xls file, compile it and set the contents
    else if (doc_type == 2)
    {
        compile_xls(file_path);
    }
}

// ----------------------------------------------------------------------------
// int set_uri( );
// ----------------------------------------------------------------------------
int xzes::Document::set_uri( xzes::uri_t file_path )
{
    uri = file_path ;
    //return status for debug
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
    xzes::uri_t file_path = uri;
    return file_path;
}

// ----------------------------------------------------------------------------
// int set_id( );
// Set the Unique ID of a Document.
// ----------------------------------------------------------------------------
int xzes::Document::set_id( xzes::id_t id )
{
    uid = id ;
    //return status for debug
    int status = EXIT_SUCCESS;
    return status;

}

// ----------------------------------------------------------------------------
// id_t get_id( );
// Return the unique ID of a Document.
// ----------------------------------------------------------------------------
xzes::id_t xzes::Document::get_id( )
{
    xzes::id_t obj = uid;
    return obj;
}

// ----------------------------------------------------------------------------
// int set_contents( );
// Set the DOM contents of a Document.
// ----------------------------------------------------------------------------
int xzes::Document::set_contents( xzes::dom_t content )
{
    //TODO: should add more thing about the cache
    contents = content ;
    //return status for debug
    int status = EXIT_SUCCESS;
    return status;

}

// ----------------------------------------------------------------------------
// int get_contents( );
// Return the DOM contents of a Document.
// ----------------------------------------------------------------------------
xzes::dom_t xzes::Document::get_contents( )
{
    xzes::dom_t doc = contents;
    return doc;
}


// ----------------------------------------------------------------------------
// int parse_xml;
// parse the xml file to dom document, and then store it back to document class
// ----------------------------------------------------------------------------
int xzes::Document::parse_xml( xzes::uri_t uri )
{
    //Initialized DOMparser
    xercesc_3_1::XercesDOMParser theParser;
    
    // Turn on validation and namespace support.
    theParser.setDoValidation(true);
    theParser.setDoNamespaces(true);
    
    // Parse the document
    theParser.parse(uri);
    DOMDocument *theDOM = theParser.getDocument();
    theDOM->normalize();
    
    XercesDOMSupport theDOMSupport;
    XercesParserLiaison theParserLiaison;
    
    // create a XercesDOMWrapperParsedSource for future using
    const XercesDOMWrapperParsedSource parsedSource(
                                                    theDOM,
                                                    theParserLiaison,
                                                    theDOMSupport,
                                                    XalanDOMString(uri.getSystemId()));
    
    // store the paresed file to class
    set_contents(parsedSource);
    
    //maybe we should terminate DOMparser?
    //but I didn't find that function :(
    
    //return status for debug
    int status = EXIT_SUCCESS;
    return status;
}

// ----------------------------------------------------------------------------
// int compile_xls( dom_t content );
// compile the stylesheet for future reuse, and store it back to document class
// ----------------------------------------------------------------------------
int xzes::Document::compile_xls( xzes::uri_t uri )
{
    //Using xalan to complie stylesheet to binary file
    XalanCompiledStylesheet* compiledStylesheet = 0;
    compiledStylesheet = theXalanTransformer.compileStylesheet(uri);
    
    //after compiled stylesheet, store it back to our class
    Document::set_contents(*compiledStylesheet);
    
    //return status for debug
    int status = EXIT_SUCCESS;
    return status;
}

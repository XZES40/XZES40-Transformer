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
// Filename: lib.hpp
// Description: Declaration of custom types, classes, and functions used in
// XZES40 Transformer
//
// All class definitions are documented in src/lib.cpp
///////////////////////////////////////////////////////////////////////////////

#include <string>

// Include Xerces and Xalan libraries.
#include <xalanc/XalanTransformer/XalanTransformer.hpp>

// Set Xerces and Xalan namespace for methods and classes.
XALAN_USING_XALAN( XalanParsedSource       );
XALAN_USING_XALAN( XalanCompiledStylesheet );

#if !defined lib
#define lib

#define SUCCESS 1
#define FAILURE 0

namespace xzes
{
    // ------------------------------------------------------------------------
    // typedef uri_t
    //
    // A simple abstraction over the type chosen to represent a file URI.
    // ------------------------------------------------------------------------
    typedef struct
    {
        std::string uri;
    } uri_t;

    // ------------------------------------------------------------------------
    // typedef struct cli_arguments
    //
    // Parses input of the following format:
    //   `<binary> <input xml> <input xsl> <out xml>`
    // into a struct of the following format:
    //   string cli_argument.xml -> <input xml>
    //   string cli_argument.xsl -> <input xsl>
    //   string cli_argument.out -> <input out>
    // ------------------------------------------------------------------------
    typedef struct
    {
        uri_t xml; // XML file location
        uri_t xsl; // Stylesheet file location
        uri_t out; // Output file location
    } cli_arguments_t ;

    // ------------------------------------------------------------------------
    // typedef id_t 
    //
    // API for obscuring how we implement the ID/specific has we are dealing
    // with.
    // ------------------------------------------------------------------------
    typedef struct
    {
        std::string id;
    } id_t;

    // ------------------------------------------------------------------------
    // typedef dom_t 
    //
    // A simple abstraction over the Xerces DOM object.
    // ------------------------------------------------------------------------
    typedef struct
    {
        // Xerces DOM object
        const XalanParsedSource *obj;
    } dom_t;

    // ------------------------------------------------------------------------
    // typedef style_t 
    //
    // A simple abstraction over the Xerces StyleSheet object.
    // ------------------------------------------------------------------------
    typedef struct
    {
        // Xerces DOM object
        const XalanCompiledStylesheet *obj;
    } style_t;

    cli_arguments_t* parse_args( int*, char*** );
    bool _file_exists( std::string );
    std::string _hash( std::string );
}

#endif

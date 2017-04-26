///////////////////////////////////////////////////////////////////////////////
// Copyright 2017, the Oregon State University Capstone Project
// 'XZES40-Transformer' with the Apache Software Foundation
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
// XZES40-Transformer
//
// All class definitions are documented in src/lib.cpp
///////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <getopt.h>
#include <functional>
#include <cstdlib>
#include <pthread.h>
#include <fstream>

// Include Xerces and Xalan libraries.
#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xercesc/util/PlatformUtils.hpp>

// Set Xerces and Xalan namespace for methods and classes.
XALAN_USING_XALAN( XSLTInputSource       );
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
    // typedef id_t 
    //
    // API for obscuring how we implement the ID/specific has we are dealing
    // with.
    // ------------------------------------------------------------------------
    typedef struct
    {
        //std::string id;
        int id;
    } id_t;

    // ------------------------------------------------------------------------
    // typedef doc_t 
    //
    // A simple abstraction over the Xerces DOM object.
    // ------------------------------------------------------------------------
    typedef struct {
        const XSLTInputSource* obj;
    } doc_t;

    bool _file_exists( std::string );
    int setup_connection();
    int valid_request(char *);
    std::vector<std::string> split(std::string, char);
    int _hash( const char* );
}

#endif

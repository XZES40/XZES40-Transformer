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
// Filename: transform.hpp
// Description: Declaration of custom structs, functions, and classes used in
// XZES40 Transformer document handling pipeline.
///////////////////////////////////////////////////////////////////////////////

#include <lib.hpp>

// xalan library for compile xls
#include <xalanc/XalanTransformer/XalanTransformerDefinitions.hpp>
// xercesc libary for parse xml
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xalanc/XalanTransformer/XercesDOMWrapperParsedSource.hpp>

#if !defined document
#define document

namespace xzes
{
    class Document
    {
        private:
            // content also store the compiled xls file,
            // but it is binary files, not DOM object.
            dom_t contents;
            id_t  uid;
            uri_t uri;

        public:
            // constructor
            Document( xzes::uri_t , int);
            int set_uri( uri_t );
            int set_contents( dom_t );
            int set_id( id_t );
            uri_t get_uri( );
            dom_t get_contents( );
            id_t  get_id( );
        
            int parse_xml( xzes::uri_t );
            int compile_xls( xzes::uri_t );
        
        
    };
}

#endif

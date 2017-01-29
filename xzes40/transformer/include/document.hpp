//////////////////////////////////////////////////////////////////////////////
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
// Filename: transform.hpp
// Description: Declaration of custom structs, functions, and classes used in
// XZES40 Transformer document handling pipeline.
///////////////////////////////////////////////////////////////////////////////

#include <lib.hpp>

#if !defined document
#define document

#define XML_DOC_TYPE 1
#define XSL_DOC_TYPE 2

namespace xzes
{
    class Document
    {
        private:
            dom_t   xml;
            style_t xsl;

            id_t  uid;
            uri_t uri;

            int doc_type;

        protected:
            int set_uri( uri_t );
            int set_id( );
            int set_type( int );

            int set_xml_content( dom_t );
            int set_xsl_content( style_t );

            id_t _hash_uri( );
            int _compile_xsl( );
            int _compile_xml( );

        public:
            Document( uri_t , int );
            Document( );

            uri_t   get_uri( );
            id_t    get_id( );

            dom_t   get_xml_content( );
            style_t get_xsl_content( );

            int compile( );
    };
}

#endif

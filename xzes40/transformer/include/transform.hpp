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
// XZES40 Transformer transformation pipeline.
///////////////////////////////////////////////////////////////////////////////

#include <string>

// This libary for xalanc++
#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>

// xercesc libary for parse xml
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xalanc/XalanTransformer/XercesDOMWrapperParsedSource.hpp>

// compile the xls
#include <XalanCompiledStylesheet.hpp>


#include <lib.hpp>
#include <document.hpp>


#if !defined transform
#define transform

namespace xzes
{
    // ------------------------------------------------------------------------
    // class Transformer
    //
    // May perform the following operations:
    // - Parse an xml file into a Document.
    // - Parse an xsl file into a Document.
    // - Transform the xml and xsl Document into the out Document.
    // ------------------------------------------------------------------------
    class Transformer
    {
        private:
            Document xml;
            Document xsl;
            // out only need uri, so I change it as string for output.
            xzes::uri_t out;

        public:
            Transformer( );
            Document transformation( xzes::Transformer );
            Document pass_xml( xzes::uri_t );
            Document pass_xsl( xzes::uri_t );
            int set_output( xzes::uri_t );
    };

    int transform_documents( cli_arguments_t* );
}

#endif

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
// Filename: transform.hpp
// Description: Implementation of custom types, classes, and functions used in
// XZES40 Transformer transformation functionality.
//
// All class declarations are documented in lib/transform.hpp.
///////////////////////////////////////////////////////////////////////////////

#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>

#include <cstdlib>
#include <iostream>

#include <transform.hpp>
#include <lib.hpp>

//define namespace
XALAN_USING_XERCES(XMLPlatformUtils);
XALAN_USING_XALAN(XalanTransformer);
XALAN_USING_XALAN(XSLTInputSource);
XALAN_USING_XALAN(XSLTResultTarget);

int xzes::transform_documents( xzes::cli_arguments_t *args )
{
    //Initialize function
    XMLPlatformUtils::Initialize();
    XalanTransformer::initialize();

    //create a xalantransformer
    XalanTransformer theXalanTransformer;

    int theResult = theXalanTransformer.transform( args->xml.uri.c_str(),
                                                   args->xsl.uri.c_str(),
                                                   std::cout);

    //terminate xalan
    XalanTransformer::terminate();
    XMLPlatformUtils::Terminate();
    XalanTransformer::ICUCleanUp();

    return theResult;
}

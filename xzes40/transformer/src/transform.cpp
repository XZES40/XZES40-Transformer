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

#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include <cstdlib>
#include <iostream>

#include <document.hpp>
#include <transform.hpp>
#include <lib.hpp>

//define namespace
XALAN_USING_XALAN(XalanTransformer);

#include <transform.hpp>

int xzes::transform_documents( xzes::job_t *args )
{
    //create a xalantransformer
    XalanTransformer theXalanTransformer;

    //Allocate objects on the heap so they can be cached in the non-prototype version.
    //XSLTInputSource  *xml = cache.get(args.xml);
    Document xml(args->xml);
    Document xsl(args->xsl);

    //The outName is the varb of output name
    //TODO -- create random output name
    std::string outName = "yooo.xml";
    XSLTResultTarget *out = new XSLTResultTarget(outName.c_str());


    int theResult = theXalanTransformer.transform( *xml.get_content()->obj ,
                                                   *xsl.get_content()->obj ,
                                                    *out );

    return theResult;
}

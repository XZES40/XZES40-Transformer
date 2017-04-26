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
// Filename: .cpp
// Description: Main entrypoint for the XZES40 Transformer application
///////////////////////////////////////////////////////////////////////////////
//
// NOTICE: THIS FILE HAS BEEN DEPRECATED IN FAVOR OF 'main.py'
// IT REMAINS HERE FOR POSTERITY BUT IS NOT MAINTAINED AND SHOULD NOT BE LOOKED
// AT AS A SOURCE OF TRUTH.
//
///////////////////////////////////////////////////////////////////////////////

#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>

#include <cstdlib>
#include <iostream>

#include <lib.hpp>
#include <document.hpp>
#include <keylist.hpp>
#include <cache.hpp>

XALAN_USING_XERCES(XMLPlatformUtils);
XALAN_USING_XALAN(XalanTransformer);

int main(){
	XMLPlatformUtils::Initialize();
	XalanTransformer::initialize();
	uri_t xml1, xsl1;
	xml1.uri = "./examples/simple.xml";
	xsl1.uri = "./examples/simple.xsl";

	Document xmlDoc1 (xml1);
	Document xslDoc1 (xsl1);

	uri_t xml2, xsl2;
	xml2.uri = "./examples/simple.xml";
	xsl2.uri = "./examples/simple.xsl";

	Document xmlDoc2 (xml2);
	Document xslDoc2 (xsl2);

	XalanTransformer theXalanTransformer;

	int theResult = theXalanTransformer.transform(*xmlDoc2.get_content()->obj,
												  *xslDoc2.get_content()->obj,
												  std::cout);
	XMLPlatformUtils::Terminate();
	XalanTransformer::ICUCleanUp();

	return theResult;
}

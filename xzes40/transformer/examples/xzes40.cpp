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

//Web link: http://xml.apache.org/xalan-c/usagepatterns.html#xalantransformer


#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>

//define namespace
XALAN_USING_XERCES(XMLPlatformUtils);
XALAN_USING_XALAN(XalanTransformer);
XALAN_USING_XALAN(XSLTInputSource);
XALAN_USING_XALAN(XSLTResultTarget);

int main( int argc, char * argv[] ){
	//Initialize function	
	XMLPlatformUtils::Initialize();
	XalanTransformer::initialize();
	//create a xalantransformer
	XalanTransformer theXalanTransformer;
	//perform each transformation	
	XSLTInputSource xmlIn("simple.xml");
	XSLTInputSource xslIn("simple.xsl");
	XSLTResultTarget xmlOut("simple-out.xml");
	int theResult =
		    theXalanTransformer.transform(xmlIn,xslIn,xmlOut);
	//terminate xalan
	XMLPlatformUtils::Terminate();
	XalanTransformer::ICUCleanUp();

	return theResult;
		
}

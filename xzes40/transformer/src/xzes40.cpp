#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <iostream>

XALAN_USING_XERCES(XMLPlatformUtils)
XALAN_USING_XALAN(XalanTransformer)
XALAN_USING_XALAN(XSLTInputSource)
XALAN_USING_XALAN(XSLTResultTarget)

////
// XZES40 Xerces C++ proof of concept.
// Segmentation faults, but compiles correctly.
// TODO: Fix that.
////

int main( int argc , char* argv[] )
{

    XMLPlatformUtils::Initialize();
    XalanTransformer::initialize();

    XalanTransformer the_xalan_transformer;

    XSLTInputSource xmlIn("test/foo.xml");
    XSLTInputSource xslIn("test/foo.xsl");
    XSLTResultTarget xmlOut("test/foo.out");

    std::cout <<  "before\n";

    int theResult = the_xalan_transformer.transform(xmlIn, xslIn, xmlOut);

    std::cout << "after. TheResult: " << theResult << "\n";

    XalanTransformer::terminate();
    XMLPlatformUtils::Terminate();

    std::cout << "last\n";

    return 0;
}

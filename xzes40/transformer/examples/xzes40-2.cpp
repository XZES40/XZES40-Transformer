#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>
//#include <xercesc/dom/DOMDocument.hpp>
//#include <xercesc/dom/DOMImplementation.hpp>

//#include <xalanc/XMLSupport/FormatterToXML.hpp>
// #include <xalanc/XMLSupport/XalanTransformer.hpp>

//define namespace
XALAN_USING_XERCES(XMLPlatformUtils);
XALAN_USING_XALAN(XalanTransformer);
XALAN_USING_XALAN(XSLTInputSource);
XALAN_USING_XALAN(XSLTResultTarget);
XALAN_USING_XALAN(XalanParsedSource);
XALAN_USING_XALAN(XalanCompiledStylesheet);

int main( int argc, char * argv[] ){
    //Initialize function
    XMLPlatformUtils::Initialize();
    XalanTransformer::initialize();

    //create a xalantransformer
    XalanTransformer theXalanTransformer;

    // Allocate objects on the heap so they can be cached in the non-prototype version.
    // XSLTInputSource  *xml = cache.get(args.xml);
    // Document xml(args.xml, XML_DOC_TYPE);
    XSLTInputSource  *xml = new XSLTInputSource("foo.xml");
    XSLTInputSource  *xsl = new XSLTInputSource("foo.xsl");
    XSLTResultTarget *out = new XSLTResultTarget("foo.out");

    int theResult = theXalanTransformer.transform(*xml,*xsl,*out);

    //terminate xalan
    XMLPlatformUtils::Terminate();
    XalanTransformer::ICUCleanUp();

    return theResult;

}

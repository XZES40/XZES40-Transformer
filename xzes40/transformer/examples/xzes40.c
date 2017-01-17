#include <xalanc/XalanTransformer/XalanCAPI.h>

#define TRUE 1
#define FALSE !(TRUE)

int main ( int argc , char * argv[] )
{
    XalanInitialize();

    XalanHandle xalan = NULL;
    xalan = CreateXalanTransformer();

    const char * xmlfilename = "test/foo.xml";
    const char * xsltfilename = "test/foo.xml";
    const char * outputfilename = "test/foo.out";
    int theResult = XalanTransformToFile(xmlfilename,
                                         xsltfilename,
                                         outputfilename,
                                         xalan);

    XalanTerminate(TRUE);

    return theResult;
}

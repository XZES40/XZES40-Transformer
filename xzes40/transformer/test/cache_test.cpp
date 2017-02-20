#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>

#include <cstdlib>
#include <iostream>

#include <lib.hpp>
#include <document.hpp>
#include <keylist.h>
#include <cache.hpp>

using namespace xzes;
XALAN_USING_XALAN(XSLTInputSource);

int main(){
	uri_t uri1, uri2, uri3;
	uri1.uri = "This is paht1";
	uri2.uri = "hahahhashdsah";
	uri3.uri = "hopework";
	Document firstElement  (uri1);
	Document secondElement (uri2);
	Document thirdElement  (uri3);
	//XSLTInputSource 1st;
	//XSLTInputSource 2nd;
	//XSLTInputSource 3rd;

	cout << "This is first path name:" << firstElement.get_uri().uri << endl;
	cout << "This is first path id:" << firstElement.get_id().id<< endl;
	cout << "This is first path doc:" << firstElement.get_content().obj<< endl;

	cout << "This is second path name:" << secondElement.get_uri().uri << endl;
	cout << "This is second path id:" << secondElement.get_id().id<< endl;
	cout << "This is second path doc:" << secondElement.get_content().obj<< endl;

	cout << "This is third path name:" << thirdElement.get_uri().uri << endl;
	cout << "This is third path id:" << thirdElement.get_id().id<< endl;
	cout << "This is third path doc:" << thirdElement.get_content().obj<< endl;

	Cache testList;
	testList.set(firstElement);
	testList.set(secondElement);
	testList.set(thirdElement);

	testList.print_name();
	testList.print_id();


	return 0;
} 


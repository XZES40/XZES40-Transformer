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
#include <arpa/inet.h>
#include <string.h>

#include <document.hpp>
#include <transform.hpp>

//define namespace
XALAN_USING_XALAN(XalanTransformer);
XALAN_USING_XALAN(XalanDOMString);


void* xzes::transform_documents( void* input )
{

	xzes::job_t* args = (job_t*) input;
	int* status = new int;

    printf("%s, %s, %s\n", args->xml.uri.c_str(),
                           args->xsl.uri.c_str(),
                           args->out.uri.c_str());

    //create a xalantransformer
    XalanTransformer theXalanTransformer;

    //catch the cache
    Cache::Cache* cacheList = args->theList;
    //pass the mute
    pthread_mutex_t mutex = args->lock_var;

    /*
    //set the parameter pass
    //XalanDOMString param[ args->parame.size() ];
    std::string param[ args->parame.size() ];
    //copy the data to local
    for (int i = 0; i < args->parame.size(); i++){
        param[i] = args->parame[i];
    }
    */


    // Allocate objects on the heap so they can be cached in the non-prototype version.
    // XSLTInputSource  *xml = cache.get(args.xml);
    Document xml(args->xml, cacheList, mutex);
    Document xsl(args->xsl, cacheList, mutex);

    puts("setup documents");

    std::string outName = args->out.uri;
    XSLTResultTarget *out = new XSLTResultTarget( outName.c_str() );

    for (int i = 0 ; i< args->parame.size() - 4; i++){
        printf("in transform array: %s \n", args->parame[i].c_str());
    }

    puts("setup the parameter");
    for (int i = 0 ; i < args->parame.size() - 4; i = i +2 ){
        theXalanTransformer.setStylesheetParam(
            XalanDOMString(args->parame[i+1].c_str() ),
            XalanDOMString(args->parame[i].c_str() ) );
    }

    puts("about to transform");

    *status = theXalanTransformer.transform( *xml.get_content()->obj ,
                                             *xsl.get_content()->obj ,
                                             *out );

    puts("after transform");
    puts("\n");
    if (&status < 0)
        args->error = "Transformation failed";

    char ret[2048];
    sprintf(ret, "%s,%s,%s", args->jid.c_str(),
                             outName.c_str(),
                             args->error.c_str());

    send(args->socket_fd, ret, strlen(ret), 0);
    return (void*)status;
}

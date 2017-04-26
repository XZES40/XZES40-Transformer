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
// XZES40-Transformer transformation functionality.
//
// All class declarations are documented in lib/transform.hpp.
///////////////////////////////////////////////////////////////////////////////

#include <transform.hpp>

// -----------------------------------------------------------------------------
// void* transform_documents( void* input )
//
// This is the transformer for transformation
// -----------------------------------------------------------------------------
void* xzes::transform_documents( void* input )
{
    //create a local input data
	xzes::job_t* args = (job_t*) input;
	int* status = new int;

    printf("%s, %s, %s\n", args->xml.uri.c_str(),
                           args->xsl.uri.c_str(),
                           args->out.uri.c_str());

    //create a xalan transformer
    XalanTransformer theXalanTransformer;

    //catch the cache
    Cache::Cache* cacheList = args->theList;

    //pass the mute
    pthread_mutex_t mutex = args->lock_var;

    //allocate objects on the heap so they can be cached in the non-prototype version.
    Document xml(args->xml, cacheList, mutex);
    Document xsl(args->xsl, cacheList, mutex);

    puts("setup documents");

    //set up the output file string
    std::string outName = args->out.uri;
    XSLTResultTarget *out = new XSLTResultTarget( outName.c_str() );

    puts("setup the parameter");
    //set up the top level parameter for the transformer
    for (int i = 0 ; i < args->param.size(); i += 1 ){
        printf("%s, %s\n", args->param[i].key.c_str(), args->param[i].val.c_str());
        theXalanTransformer.setStylesheetParam(
            XalanDOMString(args->param[i].key.c_str() ),
            XalanDOMString(args->param[i].val.c_str() )
        );
    }

    puts("about to transform");

    //start transformer
    *status = theXalanTransformer.transform( *xml.get_content()->obj ,
                                             *xsl.get_content()->obj ,
                                             *out );

    //clear the top level parameter 
    theXalanTransformer.clearStylesheetParams();

    puts("after transform");
    puts("\n");

    //feedback if we have error here
    if (&status < 0)
        args->error = "Transformation failed";

    //send error back to CGI script
    char ret[2048];
    sprintf(ret, "%s,%s,%s", args->jid.c_str(),
                             outName.c_str(),
                             args->error.c_str());

    send(args->socket_fd, ret, strlen(ret), 0);
    
    return (void*)status;
}

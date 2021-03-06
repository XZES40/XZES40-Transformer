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

#include <xalanc/XalanTransformer/XalanCAPI.h>

#define TRUE 1
#define FALSE !(TRUE)

int main ( int argc , char * argv[] )
{
    XalanInitialize();

    XalanHandle xalan = NULL;
    xalan = CreateXalanTransformer();

    const char * xmlfilename = "../test/foo.xml";
    const char * xsltfilename = "../test/foo.xml";
    const char * outputfilename = "../test/foo.out";
    int theResult = XalanTransformToFile(xmlfilename,
                                         xsltfilename,
                                         outputfilename,
                                         xalan);

    XalanTerminate(TRUE);

    return theResult;
}

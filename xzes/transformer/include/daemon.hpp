///////////////////////////////////////////////////////////////////////////////
// Copyright 2017, the Oregon State University Capstone Project
// 'XZES40-Transformer' with the Apache Software Foundation
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
// Filename: daemon.hpp
// Description: Function declarations form the Daemon.
//
// All function definitions are documented in src/daemon.cpp
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <stdlib.h>
#include <csignal>
#include <pthread.h>
#include <vector>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>


#include <parse.hpp>
#include <cache.hpp>
#include <lib.hpp>
#include <transform.hpp>


// Xalan part
#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xercesc/util/PlatformUtils.hpp>

XALAN_USING_XERCES(XMLPlatformUtils);
XALAN_USING_XALAN(XSLTInputSource);
XALAN_USING_XALAN(XSLTResultTarget);
XALAN_USING_XALAN(XalanParsedSource);
XALAN_USING_XALAN(XalanCompiledStylesheet);
XALAN_USING_XALAN(XalanTransformer);


namespace xzes {
    int daemon( int );
    int master_connection( int );
    job_t* recv_request( int , fd_set* );
}


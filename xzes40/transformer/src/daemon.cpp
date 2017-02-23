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
// Filename: daemon.cpp
// Description: Creattes daemon binary which waits and spawns jobs for
//   transformations.
// Mad props to:
// - https://github.com/troydhanson/network/blob/master/unixdomain/srv.c
///////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <csignal>

#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include <lib.hpp>
#include <transform.hpp>
#include <daemon.hpp>

XALAN_USING_XERCES(XMLPlatformUtils);
XALAN_USING_XALAN(XSLTInputSource);
XALAN_USING_XALAN(XSLTResultTarget);
XALAN_USING_XALAN(XalanParsedSource);
XALAN_USING_XALAN(XalanCompiledStylesheet);
XALAN_USING_XALAN(XalanTransformer);

#define BUFFER_SIZE 1024

// TODO: CHANGE THIS TO /var/run/... in the final release
const char *socket_path = "/tmp/xzes40daemon.socket";

int main(int argc, char* argv[]) {
    int fd;

    signal(SIGINT, xzes::signal_handler); 

    if ((fd = xzes::setup_connection()) == -1)
        exit(EXIT_FAILURE);

    xzes::daemon(fd);

    return 0;
}

int xzes::daemon(int fd)
{
    char buf[BUFFER_SIZE];
    int cl, rc;

    //Initialize Xalan stuff
    XMLPlatformUtils::Initialize();
    XalanTransformer::initialize();

    for (;;) {
        if ( (cl = accept(fd, NULL, NULL)) == -1) {
            perror("XZES40:: Accept error in transformer daemon.");
            continue;
        }

        while ( (rc = read(cl, buf, sizeof(buf))) > 0) {
            if (xzes::valid_request(buf))
            {
                // TODO MULTI THREAD
                // TODO FIX THE SEGFAULT
                //printf("XZES40:: Read input `%s`.\n", buf);
                xzes::job_t *job = xzes::parse_request( buf );
                xzes::transform_documents(job);
            }
        }
        if (rc == -1) {
            perror("XZES40:: Read error.\n");
            exit(EXIT_FAILURE);
        } else if (rc == 0) {
            // printf("XZES40:: Recieved EOF. Exiting.\n");
            close(cl);
        }
    }

    // Teardown Xalan stuff
    XMLPlatformUtils::Terminate();
    XalanTransformer::ICUCleanUp();
}

// ------------------------------------------------------------------------
// Handles signals.
// Only handles SIGINT by deleting socket file.
// ------------------------------------------------------------------------
void xzes::signal_handler(int signum)
{
    // printf("Recieved signal (%d).", signum);

    if (signum == SIGINT)
        remove(socket_path);

    exit(signum);
}

// ----------------------------------------------------------------------------
// Sets up a unix socket connection and returns the file descriptor.
// ----------------------------------------------------------------------------
int xzes::setup_connection()
{
    struct sockaddr_un addr;
    int fd;

    if (( fd = socket(AF_UNIX, SOCK_STREAM, 0 )) == -1) {
        perror("XZES40:: Socket setup error in transformer daemon.");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    if (*socket_path == '\0') {
        strncpy(addr.sun_path+1, socket_path+1, sizeof(addr.sun_path)-2);
    } else {
        strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);
        unlink(socket_path);
    }

    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("XZES40:: Bind error in transformer daemon.");
        exit(EXIT_FAILURE);
    }

    if ( listen( fd, 5 ) == -1 ) {
        perror("XZES40:: Listen error in transformer daemon.");
        exit(EXIT_FAILURE);
    }

    return fd;
}

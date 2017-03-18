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
// Much thanks to:
// - https://github.com/troydhanson/network/blob/master/unixdomain/srv.c
// - http://www.binarytides.com/multiple-socket-connections-fdset-select-linux/
///////////////////////////////////////////////////////////////////////////////

#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <vector>

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

#define TRUE 1;

#define BUFFER_SIZE 1024
#define PORT 40404

int main(int argc, char* argv[]) {
    int conn, ret;

    conn = master_connection(40404);
    puts("WTF");
    if (conn > 0) {
        puts("Able to do stuff");
        ret = xzes::daemon(conn);
    } else {
        perror("XZES:: Unable to start daemon.\n");
        exit(EXIT_FAILURE);
    }

    return ret;
}

// Eventloop for receiving requests and dispatching jobs
int xzes::daemon(int master) {

    fd_set* readfds = NULL;

    printf("Starting daemon on localhost:%d\n", PORT);

    while (true) {
        xzes::job_t *j = xzes::recv_request(master, readfds);
		if (j != NULL)
			puts("do the transform");
		else
			puts("don't do the transform");
    }

    return 0;
}

// Recieves a request,
// parses request into job_t
xzes::job_t* xzes::recv_request(int conn, fd_set* ) {

	char buf[BUFFER_SIZE];
	fd_set *readfds = NULL;
	int activity;
	int addrlen;
    struct sockaddr_in address;
	int new_socket;
	xzes::job_t *tmp = NULL;

	FD_ZERO(readfds);
	FD_SET(conn, readfds);

	// TODO: Debug this section, pretty sure conn+1 should be something else.
	// `conn` should be "the highest file descriptor name.
	// Reference the second 'much thanks' link for more info.

    activity = select( conn + 1 , readfds , NULL , NULL , NULL);
    
    if ((activity < 0) && (errno!=EINTR)) 
    {
    	puts("select error");
    }

    //type of socket created
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

	if (FD_ISSET(conn, readfds)) 
	{
		if ((new_socket = accept(conn, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
		{
			perror("accept");
			exit(EXIT_FAILURE);
		} else {
			if (read(new_socket, buf, sizeof(buf)) > 0 && xzes::valid_request( buf ))
			{
				xzes::job_t *tmp = parse_request( buf );
				tmp->error = "";
				tmp->socket_fd = new_socket;
			}
		}
	}


    return tmp;
}

// Sets up a connection,
// Waits for input
int xzes::master_connection(int port) {
    int opt = TRUE;
    int master_socket;
    struct sockaddr_in address;

    //create a master socket
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    //type of socket created
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    //bind the socket to localhost port 8888
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0) 
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    //try to specify maximum of 3 pending connections for the master socket
    if (listen(master_socket, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
      
    //accept the incoming connection
    puts("Waiting for connections ...");

}

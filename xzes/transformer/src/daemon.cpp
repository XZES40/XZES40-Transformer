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

#include <daemon.hpp>

#define TRUE 1
#define BUFFER_SIZE 1024
#define PORT 40404
#define MAXTHREAD 5

pthread_mutex_t mutex;
pthread_t callThd[MAXTHREAD];

// ------------------------------------------------------------------------
// Eventloop for receiving requests and dispatching jobs
// Keep running as daemon.
// As the infinite loop.
// ------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    int conn, ret;

    conn = master_connection(PORT);
    if (conn > 0) {
        ret = xzes::daemon(conn);
    } else {
        perror("XZES:: Unable to start daemon.\n");
        exit(EXIT_FAILURE);
    }

    return ret;
}

// ------------------------------------------------------------------------
// daemon ( int fd )
//
// The major function for the daemon/
// This function contain the infinite loop and
// create the multiply threads for the parallel computation.
// ------------------------------------------------------------------------

int xzes::daemon(int fd)
{

    fd_set readfds;

    printf("Starting daemon on localhost:%d\n", PORT);

    //declare varb for the function using.
    int rc;
    void * status;
	pthread_attr_t attr;
    pthread_t thread[MAXTHREAD];
    xzes::job_t *job[MAXTHREAD], *temp = NULL;

	//Create cache for file
	Cache::Cache *storeList = new Cache();

	//Initialize pthread mutex
	pthread_mutex_init(&mutex, NULL);

    //Initialize xalan transformer
    XMLPlatformUtils::Initialize();
    XalanTransformer::initialize();

    /* Create threads to be Joinable*/
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    //Create array to handle the job data for each thread.
    for (int i = 0; i <= MAXTHREAD; i++){
        job[i] = temp;
    }

    while (true) {
    	for (int i = 0; i <=MAXTHREAD ; i++){
            job[i] = xzes::recv_request(fd, &readfds);
            puts("Read file ");
			if (job[i]!= NULL)
        	{
            	puts("pre-transform");
                    //add the cache to job object
                	job[i]->theList = storeList;
                    //pass the mutex locker to job object
                	job[i]->lock_var = mutex;
                    //Set the thread id;
                    job[i]->tid = i ;
                    //create connection
                	rc = pthread_create(&thread[i], &attr, xzes::transform_documents, (void *)job[i]);
                    //give the feedback for thread 
					if (rc){
						printf("ERROR; return code from pthread_create is %d\n", rc);
						exit(-1);
					}
					job[i] = NULL;
            	puts("post-transform");

        	} else {
				puts("don't do the transform");
        	}
    	}
        puts("after the thread loop");

        pthread_attr_destroy(&attr);
        puts("after destroy");

        //wait for other thread join
        for (int i = 0; i <= MAXTHREAD; i++ ){
            pthread_join(callThd[i], &status);
        }
        puts("after pthread join");

        pthread_mutex_destroy(&mutex);
        puts("after mutex desotry");
    }

    //Terminate the xalan transformer
    XalanTransformer::ICUCleanUp();
    XMLPlatformUtils::Terminate();

    return *((int*)(&status));;
}

// ------------------------------------------------------------------------
// job_t* recv_request(int conn, fd_set*)
//
// Recieves a request,
// parses request into job_t
// ------------------------------------------------------------------------
xzes::job_t* xzes::recv_request(int conn, fd_set* )
{

	char buf[BUFFER_SIZE];
	fd_set readfds;
	int activity;
	int addrlen;
    struct sockaddr_in address;
	int new_socket;
	xzes::job_t *tmp = NULL;
    puts("FD_ZERO, FD_SET");

	FD_ZERO(&readfds);
	FD_SET(conn, &readfds);

    puts("Post FD_ZERO, FD_SET");

	// TODO: Debug this section, pretty sure conn+1 should be something else.
	// `conn` should be "the highest file descriptor name.
	// Reference the second 'much thanks' link for more info.

    activity = select( conn + 1 , &readfds , NULL , NULL , NULL);
    
    if ((activity < 0) && (errno!=EINTR)) 
    {
    	puts("select error");
    }

    //type of socket created
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    puts("FD is set");

	if (FD_ISSET(conn, &readfds)) 
	{
        new_socket = accept(conn, (struct sockaddr *)&address, (socklen_t*)&addrlen);

        puts("Create a new socket");

		if ( new_socket < 0 )
		{
            puts("socket is small than 0");
			perror("accept");
			exit(EXIT_FAILURE);
		} else {
			if (read(new_socket, buf, sizeof(buf)) > 0 && xzes::valid_request( buf ))
			{

                puts("setting job");

				tmp = parse_request( buf );
				tmp->error = "";
				tmp->socket_fd = new_socket;

			}
            else
                puts("failed with setting job");
		}
	}


    return tmp;
}


// ------------------------------------------------------------------------
// int master_connection(int port)
// 
// Sets up a connection,
// Waits for input
// ------------------------------------------------------------------------
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
    puts("============================");
    puts("Waiting for connections ...");

    return master_socket;
}

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
// Filename: parse.hpp
// Description: header file of parse
///////////////////////////////////////////////////////////////////////////////

#include <lib.hpp>
#include <cache.hpp>

// ------------------------------------------------------------------------
// typedef struct job_t
//
// Parses input of the following format:
//   `<binary> <input xml> <input xsl> <out xml>`
// into a struct of the following format:
//   string job.xml.uri -> <input xml>
//   string job.xsl.uri -> <input xsl>
//   string job.out.uri -> <output file destination>
// As well as stores metadata about the job being processed (where the job
// is the transformation of the above files.
//   thread_t tid -> id of processing thread
//   string jid -> unique ID of the request (like a service ticket ID)
// -----------------------------------------------------------------------
#if !defined parse
#define parse


namespace xzes
{
    // ------------------------------------------------------------------------
    // typedef struct job_t
    //
    // Parses input of the following format:
    //   `<binary> <input xml> <input xsl> <out xml>`
    // into a struct of the following format:
    //   string job.xml.uri -> <input xml>
    //   string job.xsl.uri -> <input xsl>
    //   string job.out.uri -> <output file destination>
    // As well as stores metadata about the job being processed (where the job
    // is the transformation of the above files.
    //   thread_t tid -> id of processing thread
    //   string jid -> unique ID of the request (like a service ticket ID)
    // ------------------------------------------------------------------------
    typedef struct
    {
        uri_t xml; // XML file location
        uri_t xsl; // Stylesheet file location
        uri_t out; // Output file location
        std::string jid; // uniqe job id
        std::string error; // any error messages returned from the transform
        int socket_fd; // socket used to communicate with calling script
        int tid;
        xzes::Cache *theList;
        pthread_mutex_t lock_var;
    } job_t ;

    job_t* parse_request( char* );

    job_t* parse_args( int*, char*** );
}

#endif

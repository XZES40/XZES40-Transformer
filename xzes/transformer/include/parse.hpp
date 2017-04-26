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
// Filename: parse.hpp
// Description: header file of parse
///////////////////////////////////////////////////////////////////////////////

#include <lib.hpp>
#include <cache.hpp>

#if !defined parse
#define parse

namespace xzes
{
    // -----------------------------------------------------------------------
    // typedef strcut param_t
    // This datatype to express parameters
    // 
    // -----------------------------------------------------------------------

    typedef struct
    {
        std::string key;
        std::string val;   
    } param_t;

    // ------------------------------------------------------------------------
    // typedef struct job_t
    //
    // Parses input of the following format:
    //   `<binary> <input xml> <input xsl> <out xml>`
    // into a struct of the following format:
    //   string job.xml.uri -> <input xml>
    //   string job.xsl.uri -> <input xsl>
    //   string job.out.uri -> <output file destination>
    // ------------------------------------------------------------------------

    typedef struct
    {
        uri_t xml; // XML file location
        uri_t xsl; // Stylesheet file location
        uri_t out; // Output file location
        std::string jid; // uniqe job id
        std::string error; // any error messages returned from the transform
        std::vector<xzes::param_t> param; //a array contain the parameter.
        int socket_fd; // socket used to communicate with calling script
        int tid; // The thread id.
        xzes::Cache *theList; // the Cache object.
        pthread_mutex_t lock_var; // the thread mutex locker varb
    } job_t ;

    // Parses argv into a job_t struct 
    job_t* parse_request( char* );
    // Simple way to parses argv into a job_t struct
    job_t* parse_args( int*, char*** );
}

#endif

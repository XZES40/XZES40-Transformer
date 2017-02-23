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
// Filename: lib.cpp
// Description: Definition and documentation for custom structs, functions,
// and classes used in XZES40 Transformer
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <getopt.h>
#include <string>
#include <vector>
#include <sstream>

#include <lib.hpp>

// ----------------------------------------------------------------------------
// job_t * parse_args( int argc, char* argv[] )
//
// Parses argv into a job_t struct
// Assigns an argument to "NUL" ("\0") if the file does not exist.
// ----------------------------------------------------------------------------
xzes::job_t* xzes::parse_request( char* input )
{
    xzes::job_t *out = new xzes::job_t;

    std::string tmp (input);

    std::vector<std::string> tmpv = xzes::split(input,',');

    out->jid     = tmpv[0];
    out->xml.uri = tmpv[1];
    out->xsl.uri = tmpv[2];

    // printf("(%s,%s,%s)\n",out->jid.c_str(),
    //                      out->xml.uri.c_str(),
    //                      out->xml.uri.c_str());

    return out;
}

std::vector<std::string> xzes::split( std::string s, char c)
{
    std::vector<std::string> v;

    std::string::size_type i = 0;
    std::string::size_type j = s.find(c);

    while (j != std::string::npos) {
        v.push_back(s.substr(i, j-i));
        i = ++j;
        j = s.find(c, j);

        if (j == std::string::npos)
            v.push_back(s.substr(i, s.length()));
    }

    return v;
}

// ----------------------------------------------------------------------------
// job_t * parse_args( int argc, char* argv[] )
//
// Parses argv into a job_t struct
// Assigns an argument to "NUL" ("\0") if the file does not exist.
// ----------------------------------------------------------------------------
xzes::job_t* xzes::parse_args( int *argc , char* *argv[ ] )
{
    int c;
    xzes::job_t *args = new xzes::job_t;
    while ( true )
    {
        static struct option long_options[] =
        {
            { "xml", required_argument, 0, 'm' },
            { "xsl", required_argument, 0, 's' },
            { "out", required_argument, 0, 'o' },
        };

        int i = 0;
        c = getopt_long ( *argc, *argv, "mso", long_options, &i );

        if ( c == -1 )
            break;

        switch( c )
        {
            case 'm':
                if ( xzes::_file_exists( optarg ) )
                {
                    args->xml.uri = optarg;
                }
                else
                {
                    args->xml.uri = "\0";
                }
                break;

            case 's':
                if ( xzes::_file_exists( optarg ) )
                {
                    args->xsl.uri = optarg;
                }
                else
                {
                    args->xsl.uri = "\0";
                }
                break;

            case 'o':
                if ( xzes::_file_exists( optarg ) )
                {
                    args->out.uri = optarg;
                }
                else
                {
                    args->out.uri = "\0";
                }
                break;
        }
    }
    return args;
}

// ----------------------------------------------------------------------------
// bool xzes::_file_exists( string file_path )
//
// Returns True if file exists, False if file does not exist.
// ----------------------------------------------------------------------------
bool xzes::_file_exists( std::string file_path )
{
    // The filepath is non-empty
    if( file_path.length( ) )
    {
        std::ifstream infile( file_path.c_str() );
        return infile.good( );
    }
    // If the input is empty return True
    else
    {
        return true;
    }
}


// ----------------------------------------------------------------------------
// string _hash( string )
//
// Takes an input string and returns a hash of that string.
// Thanks to:
//   http://stackoverflow.com/questions/2535284/how-can-i-hash-a-string-to-an-int-using-c
// This is hopefully a temporary measure and will eventually become a wrapper
// for a *real* hash function.
// ----------------------------------------------------------------------------
std::string xzes::_hash( std::string input )
{
    int hash   = 0;
    int offset = 'a' - 1;

    for( std::string::const_iterator it=input.begin(); it!=input.end(); ++it)
    {
        hash = hash << 1 | (*it - offset);
    }

    std::ostringstream ret;
    ret << hash;

    return ret.str();
}

// ----------------------------------------------------------------------------
// Returns if a request sent via unix socket is valid.
//
// TODO: A nieve implementation, should be improved.
// ----------------------------------------------------------------------------
int xzes::valid_request(char * b)
{
    std::string tmp (b);
    // printf("%lu\n", tmp.find(","));
    if ( tmp.find(",") )
        // printf("Found `,` in input\n");
        return SUCCESS;
    else
        // printf("Could not find `,` in input\n");
        return FAILURE;
}

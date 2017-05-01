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
// and classes used in XZES40-Transformer
///////////////////////////////////////////////////////////////////////////////

#include <lib.hpp>
#include <parse.hpp>

// ---------------------------------------------------------------------------- 
// std::vector<std::string> xzes::split( std::string s, char c)
//
// This is help function for the parse_request.
// ----------------------------------------------------------------------------
std::vector<std::string> xzes::split( std::string s, char c)
{
    std::vector<std::string> v;

    std::string::size_type i = 0;
    std::string::size_type j = s.find(c);

    while (j != std::string::npos)
    {
        v.push_back(s.substr(i, j-i));
        i = ++j;
        j = s.find(c, j);

        if (j == std::string::npos)
        {
            v.push_back(s.substr(i, s.length()));
        }
    }

    return v;
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
int xzes::_hash( const char* s )
{
    unsigned int seed = 0;
    unsigned int hash = seed;

    while (*s)
    {
        hash = hash * 101  +  *s++;
    }

    return hash;

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
    {
        // printf("Found `,` in input\n");
        return SUCCESS;
    } else {
        // printf("Could not find `,` in input\n");
        return FAILURE;
    }
}

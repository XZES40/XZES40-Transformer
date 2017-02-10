///////////////////////////////////////////////////////////////////////////////
// Copyright 2017, the Oregon State University Capstone Project 'XZES40'
// with the Apache Foundation
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
#include <getopt.h>

#include <lib.hpp>

// ----------------------------------------------------------------------------
// cli_arguments parse_args( int argc, char* argv[] )
//
// Parses argv into a cli_arguments_t struct
// Assigns an argument to "NUL" ("\0") if the file does not exist.
// ----------------------------------------------------------------------------
xzes::cli_arguments_t* xzes::parse_args( int *argc , char* *argv[ ] )
{
	int c;
	xzes::cli_arguments_t *args = new xzes::cli_arguments_t;
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

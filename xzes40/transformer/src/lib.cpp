///////////////////////////////////////////////////////////////////////////////
// Filename: lib.cpp
// Description: Definition and documentation for custom structs, functions,
// and classes used in XZES40 Transformer
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include <lib.hpp>


// ----------------------------------------------------------------------------
// cli_arguments parse_args( int argc, char* argv[] )
//
// Parses argv into a cli_arguments_t struct
// ----------------------------------------------------------------------------
cli_arguments_t* parse_args( int *argc , char* *argv[] )
{
	int c;
	cli_arguments_t *args = new cli_arguments_t;
	while ( 1 )
	{
		static struct option long_options[] =
		{
			  {"xml", required_argument, 0, 'm'},
			  {"xsl", required_argument, 0, 's'},
			  {"out", required_argument, 0, 'o'},
		};

		int i = 0;
		c = getopt_long ( *argc, *argv, "mso", long_options, &i );

		if ( c == -1 )
			break;

		switch( c )
		{
			case 'm':
				args->xml = optarg;
				break;

			case 's':
				args->xsl = optarg;
				break;

			case 'o':
				args->out = optarg;
				break;
		}
	}
    return args;
}

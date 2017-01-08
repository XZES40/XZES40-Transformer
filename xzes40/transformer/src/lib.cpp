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
                    args->xml = optarg;
                }
                else
                {
                    args->xml = "\0";
                }
				break;

			case 's':
                if ( xzes::_file_exists( optarg ) )
                {
                    args->xsl = optarg;
                }
                else
                {
                    args->xsl = "\0";
                }
				break;

			case 'o':
                if ( xzes::_file_exists( optarg ) )
                {
                    args->out = optarg;
                }
                else
                {
                    args->out = "\0";
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
// int Document::Document( std::string file_path);
//
// Intializes a document by setting the uri, contents, and ID.
// ----------------------------------------------------------------------------
int Document::Document( std::string file_path )
{
    int status = EXIT_SUCCESS;
    returns status;

}

// ----------------------------------------------------------------------------
// int Document::set_uri( );
// ----------------------------------------------------------------------------
int Document::Document( )
{
    int status = EXIT_SUCCESS;
    returns status;

}

// ----------------------------------------------------------------------------
// int Document::get_uri( );
// ----------------------------------------------------------------------------
int Document::get_uri( )
{
    int status = EXIT_SUCCESS;
    returns status;

}

// ----------------------------------------------------------------------------
// int Document::set_contents( );
// ----------------------------------------------------------------------------
int Document::set_contents( )
{
    int status = EXIT_SUCCESS;
    returns status;

}

// ----------------------------------------------------------------------------
// int Document::get_contents( );
// ----------------------------------------------------------------------------
int Document::get_contents( )
{
    int status = EXIT_SUCCESS;
    returns status;

}

// ----------------------------------------------------------------------------
// int Document::set_id( );
// ----------------------------------------------------------------------------
int Document::set_id( )
{
    int status = EXIT_SUCCESS;
    returns status;

}

// ----------------------------------------------------------------------------
// int Document::get_id( );
// ----------------------------------------------------------------------------
int Document::get_id( )
{
    int status = EXIT_SUCCESS;
    returns status;

}


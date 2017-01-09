///////////////////////////////////////////////////////////////////////////////
// Filename: lib.hpp
// Description: Declaration of custom types, classes, and functions used in
// XZES40 Transformer
//
// All class definitions are documented in src/lib.cpp
///////////////////////////////////////////////////////////////////////////////

#include <string>

#if !defined lib
#define lib

namespace xzes
{
    // ------------------------------------------------------------------------
    // typedef struct cli_arguments
    //
    // Parses input of the following format:
    //   `<binary> <input xml> <input xsl> <out xml>`
    // into a struct of the following format:
    //   string cli_argument.xml -> <input xml>
    //   string cli_argument.xsl -> <input xsl>
    //   string cli_argument.out -> <input out>
    // ------------------------------------------------------------------------
    typedef struct
    {
        std::string xml; // XML file location
        std::string xsl; // Stylesheet file location
        std::string out; // Output file location
    } cli_arguments_t ;

    // ------------------------------------------------------------------------
    // typedef hash_t
    //
    // API for obscuring what the hash is that we are dealing with.
    // Possibly a temporary measure.
    // ------------------------------------------------------------------------
    typedef struct
    {
        int value;
    } hash_t;

    cli_arguments_t* parse_args( int*, char*** );
    bool _file_exists( std::string );
}

#endif

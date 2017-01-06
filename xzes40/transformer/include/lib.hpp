///////////////////////////////////////////////////////////////////////////////
// Filename: lib.hpp
// Description: Declaration of custom structs, functions, and classes used in
// XZES40 Transformer
///////////////////////////////////////////////////////////////////////////////

#include <string>

#if !defined lib
#define lib

// ----------------------------------------------------------------------------
// struct cli_arguments
//
// Parses input of the following format:
//   `<binary> <input xml> <input xsl> <out xml>`
// into a struct of the following format:
//   string cli_argument.xml -> <input xml>
//   string cli_argument.xsl -> <input xsl>
//   string cli_argument.out -> <input out>
// ----------------------------------------------------------------------------
struct cli_arguments_t
{
    std::string xml; // XML file location
    std::string xsl; // Stylesheet file location
    std::string out; // Output file location
};

cli_arguments_t* parse_args( int *argc , char* *argv[] );

#endif

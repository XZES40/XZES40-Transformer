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
    // typedef uri_t
    //
    // A simple abstraction over the type chosen to represent a file URI.
    // ------------------------------------------------------------------------
    typedef struct
    {
        // TODO decide on actual type. Not int.
        std::string uri;
    } uri_t;

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
        uri_t xml; // XML file location
        uri_t xsl; // Stylesheet file location
        uri_t out; // Output file location
    } cli_arguments_t ;

    // ------------------------------------------------------------------------
    // typedef id_t 
    //
    // API for obscuring how we implement the ID/specific has we are dealing
    // with.
    // ------------------------------------------------------------------------
    typedef struct
    {
        // TODO decide on actual type. Not int.
        int member;
    } id_t;

    // ------------------------------------------------------------------------
    // typedef dom_t
    //
    // A simple abstraction over the Xerces/Xalan DOM objects.
    // ------------------------------------------------------------------------
    typedef struct
    {
        // TODO decide on actual type. Not int.
        int dom;
    } dom_t;

    cli_arguments_t* parse_args( int*, char*** );
    bool _file_exists( std::string );
}

#endif

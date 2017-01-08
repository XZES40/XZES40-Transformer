///////////////////////////////////////////////////////////////////////////////
// Filename: lib.hpp
// Description: Declaration of custom structs, functions, and classes used in
// XZES40 Transformer
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
    } cli_arguments_t;

    // ------------------------------------------------------------------------
    // class Document
    // 
    // Stores all data about documents and functions to act on that data.
    // Data includes:
    // - Document contents (Parsed XML/XSL document, not raw)
    // - Document ID  (Unique identifier for document)
    // - Document URI (Path to find file)
    // Functions include:
    // - set_id( )
    // - set_uri( )
    // - set_uri( ) 
    // ------------------------------------------------------------------------
    class Document
    {
        private:
            int contents;
            int id;
            int uri;

        public:
            Document( std::string );

            int set_id( );
            int get_id( );

            int set_contents(  );
            int get(  );

            int set_uri( std::string );
            int get_uri( );
    };


    cli_arguments_t* parse_args( int*, char*** );
    bool _file_exists( std::string );
}

#endif

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
// Filename: parse.cpp
// Description: parse the GCI script
///////////////////////////////////////////////////////////////////////////////

#include <parse.hpp>

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
    out->out.uri = tmpv[3];

    // Stream ends in an empty "," for buffer cruft.
    for (int i = 4 ; i+2 < tmpv.size(); i += 2){
        param_t x = {tmpv[i], tmpv[i+1]};
        printf("%s:%s\n", x.key.c_str(), x.val.c_str());
        out->param.push_back(x);
    }

    // printf("(%s,%s,%s)\n",out->jid.c_str(),
    //                      out->xml.uri.c_str(),
    //                      out->xml.uri.c_str());

    return out;
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

// copy in binary mode
bool copyFile(const char *SRC, const char* DEST)
{
    std::ifstream src(SRC, std::ios::binary);
    std::ofstream dest(DEST, std::ios::binary);
    dest << src.rdbuf();
    return src && dest;
}

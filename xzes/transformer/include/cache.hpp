///////////////////////////////////////////////////////////////////////////////
// Copyright 2017, the Oregon State University Capstone Project
// 'XZES40-Transformer' with the Apache Software Foundation
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
// Filename: cache.hpp
// Description: Declaration of custom structs, functions, and classes used in
// XZES40-Transformer cacheing pipeline.
//
// All class definitions are documented in src/cache.cpp
///////////////////////////////////////////////////////////////////////////////

#include <lib.hpp>
#include <keylist.hpp>

#if !defined cache
#define cache

namespace xzes
{
    class Cache {
        private:
            KeyListEntry *theList;

        public:
            Cache();         
            bool search(id_t);
            doc_t* get( id_t );
            int set( id_t,doc_t*,uri_t );
            int print_name();
            int    print_id();

    };
}

#endif

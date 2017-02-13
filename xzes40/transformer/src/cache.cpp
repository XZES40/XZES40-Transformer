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
// Filename: cache.cpp
// Description: Definition and documentation for custom functions,
// and classes used in XZES40 Transformer cacheing pipeline.
///////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <cache.hpp>
#include <lib.hpp>

// --------------------------------------------------------------------
// Document* Cache::get( Document* )
//
// Attempts to get a Document from the cache.
// Returns the object if it is in the cache.
// Returns NULL if it does not exist in the cache.
// --------------------------------------------------------------------
xzes::Document* xzes::Cache::get( xzes::Document* object )
{
    // TODO: Write this function
    return object;
}

// --------------------------------------------------------------------
// Document* Cache::set( Document* )
//
// Attempts to put a Document object in the in-memory cache.
// Returns the object if successful.
// Returns NULL if not successful.
// --------------------------------------------------------------------
xzes::Document* xzes::Cache::set( xzes::Document* object )
{
    // TODO: Write this function
    return object;
}

// --------------------------------------------------------------------
// int Cache::del( Document* )
//
// Deletes an object from the cache.
// Returns SUCCESS if the object was delete successfully.
// Returns SUCCESS if the object was not in the cache (still does
// not exist in cache).
// Returns FAILURE if the object is in the cache but was not
// deleted.
// --------------------------------------------------------------------
int xzes::Cache::del( xzes::Document* object )
{
    // TODO: Write ths function
    int status = SUCCESS;
    return status;
}

# Cache

The caching component of XZES40 Transformer is designed to create an easily usable Caching API for the Transformer and Document components.

The cache is using the XercessC keyList to manager the cache. The data structure of the keyList is link list. Although link list is little slowly than hash map, it provide by XercessC Library, so we decided to use it as our cache.

The current interface looks like this:

#include <lib.hpp>
#include <keylist.hpp>
#include <cstdlib>

	class Cache {
        private:
        	KeyListEntry *theList;

        public:
        	Cache();
            bool search(id_t);
            doc_t* get( id_t );
            int set( id_t,doc_t*,uri_t );
            int print_name();
            int	print_id();

    };


To clarify:
- `Cache()` which is the default constructor.
- `bool search(id_t)` will search the content inside of cache, return true if there is file exist.
- `doc_t* (id_t)` return the content from the cache.
- `int set(id_t,doc_t*,uri_t)` Set the content to cache. id_t is id, uri_t is name, doc_t is content.
- `int print_name()` is a debug tools for printing all content from the cache.
- `int print_id()` is a debug tools for printing all id from the cache.

#Usage
To using cache, user also need document file, because `Document` will parse the file and hash the id for cache. 
The data flow is blow:
1.`Document` set the uri, and hash the id for 1 document.
2.`Document` check if there is file exist in the cache.
3. if true, run cache.get and document.set_content for `Document` class.
4. if false, run document.complie(), and cache_set() for `Cache` storing.
5. Done


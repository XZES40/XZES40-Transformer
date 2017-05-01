# Cache

The caching component of XZES40-Transformer is designed to create an easily usable Caching API for the Transformer and Document components.

Cache uses the XercessC keyList to manage the stored state.
The data structure of the keyList is link list.
Although link list is slower than hash map, it was used becuas it was provided by the XercessC Library.

## API

The current interface looks like this:

```cpp
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
```

To clarify:

- `Cache()` which is the default constructor.
- `bool search(id_t)` will search the content inside of cache, return true if there is file exist.
- `doc_t* (id_t)` return the content from the cache.
- `int set(id_t,doc_t*,uri_t)` Set the content to cache. id_t is id, uri_t is name, doc_t is content.
- `int print_name()` is a debug tools for printing all content from the cache.
- `int print_id()` is a debug tools for printing all id from the cache.

# Usage

The only component of the Cache which is used is for storing Documents.
When a document is created it is parsed and cached automatically, given an input file.

The data flow is as follows:

1. The `Document` constructor sets the URI and hash for the given document.
2. The `Document` constructor checks if the document is already in the cache for the hash key.
    - if true, run cache.get and document.set_content for `Document` class.
    - if false, run document.complie(), and cache_set() for `Cache` storing.
5. Done

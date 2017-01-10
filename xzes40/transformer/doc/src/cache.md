# Cache

The caching component of XZES40 Transformer si designed to create an easily usable Caching API for the Transformer and Document components.

The underlying implementation of the Caching system may be done using [Redis][redis], a database, or an in-memory cache managed by a daemon.
Either way, the excessable API should be simple to use and maintain so that changes to the caching system do not require refactoring of the Transformer and Document components.

The current (rough) interface looks like this:

```
#include <cache.hpp>
#include <document.hpp>
#include <lib.hpp>

void f( Document d )
{
    Cache c = Cache( );  // Cache is setup *completely* setup in this step.

    // Check if the Document `d` is in the cache
    if ( c.get( d.id ) == FAILURE )
        cout << d.uri << " is not in the cache" << endl;

    // Put Document `d` in the cache.
    if ( c.put( d.id, d ) )
        cout << d.uri << " was put in the cache successfully." << endl;

    // Remove Document `d` from the cache.
    if ( c.del( d.id, d ) )
        cout << d.uri << " was removed from the cache successfully." << endl;
}
```

To clarify:

- `Cache::get( string id )` returns class `Document` (or a failure).
- `Cache::put( string key, Document value )` returns a `bool` (success or failure).
- `Cache::del( string key )` returns a `bool` (success or failure).

## Non-caching documents

When an ID of `NULL` is passed to `get`, the Cache returns a placeholder Document.
This is almost exclusively used for outputting a document to stdout or a new file.
The owner of the document must *manually* cache the document for it to be stored in the future.

[redis]: https://redis.io/

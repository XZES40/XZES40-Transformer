# Application Daemon

**NOTE** In production the daemon is called `xzes40d` and is stored on disk at `/usr/local/bin/xzes40d`.

In order to preserve state in the in-memory cache, XZES has a long-running daemon.
This could have been implemented a number of ways: Redis, a database, carrier pigeons.
We chose an in memory cache as the volatility is not hyper detrimental to the goals of the application (convenience, then speed).

Here is roughly how that daemon works and how it recieves jobs.

```
 +----------------------+
 |                      |
 |   Main Executable    |
 |                      |
 +--^----------------+--+
    | network socket |
+-------------------------------------------------------+
|   |                |                                  |
|   |                |                                  |
|   |                |         +--------------------+   |
|   |              +----------->                    |   |
|   |                          |   Transformer      |   |
|   +--------------------------+                    |   |
|                              +----+--------^------+   |
|   Daemon Process                  |        |          |
|                                   |        |          |
|  +--------------------------------v--------+------+   |
|  |                                                |   |
|  |   Cache                                        |   |
|  |                                                |   |
|  +------------------------------------------------+   |
|                                                       |
+-------------------------------------------------------+
```

## Usage

The daemon is included as a systemd service in the application's codebase (`xzesd.service`).
This service is setup and enabled in the `vagrant` setup script.

The daemon can be started manually with the following commands:

```
$ make             # Build the daemon
[... successful build ...]
$ ./build/xzesd &  # spawns a daemon process in the background
[1] 14877
```

... and tested like so:

```
$ ./build/main.py `pwd`/examples/simple.xml `pwd`/examples/simple.xsl # check that the dameon is running
<?xml version="1.0" encoding="UTF-8"?><out>Hello</out>
```

This assumes that the application is not running with the web-app component. To test that see the `cgi-bin` directory's README.

## The Daemon process startup

1. When the `daemon` binary is executed it sets up a long running process and goes into a state of cybernation.
2. This process *will* start by:
  - Opening and listening on a `localhost:8000`
  - Allocating a block of memory for the cache.
  - Spawning a set of threads on which transformations may occur.

## Spawning a Transformation

1. When the `main.py` script recieves a request it is passed two files, input xml and xslt file-paths, as well as some parameters.
   This is either done via the web API (tested with `simple-test.sh` or a CLI directly talking to the daemon... probably the first of those two.
2. The `main.py` script opens a local socket connection with the daemon process.
3. The request is passed in a tuple with the following format: `(request-id#,input-filename.xml,input-filename.xsl)`.
4. The daemon process the arguments checking for errors.
5. The daemon process spawns a thread and calls the `transform_documents` method in that thread.
6. Once the thread completes the transofrmation it returns a status code which is propagated up to the `main.py` script and returns to the calling process.

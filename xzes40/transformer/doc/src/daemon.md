# Application Daemon

In order to preserve state in the in-memory cache, XZES has a long-running daemon.

Here is roughly how that daemon works and how it recieves jobs.

```
 +----------------------+
 |                      |
 |   Main Executable    |
 |                      |
 +--^--------------+----+
    | unix sockets |
+-------------------------------------------------------+
|   |              |                                    |
|   |              |                                    |
|   |              |           +--------------------+   |
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

Eventually the daemon will be included in a systemd service file and so it will not *need* to be manually started.
Until then it can be spawned like so:

```
$ make
[... successful build ...]
$ ./build/daemon &      # spawns a daemon process in the background
[1] 14877
```

... and tested like so:

```
$ ./build/main.py `pwd`/examples/simple.xml `pwd`/examples/simple.xsl # check that the dameon is running
<?xml version="1.0" encoding="UTF-8"?><out>Hello</out>
```

## The Daemon process startup

1. When the `daemon` binary is executed it sets up a long running process and goes into a state of cybernation.
2. This process *will* start by:
  - Opening and listening on a unix socket `/var/run/xzes40daemon.socket`.
  - Allocating a block of memory for the cache.
  - Spawning a set of threads on which transformations may occur.

## Spawning a Transformation

1. When the `main.py` script recieves a request it is passed two files, input xml and xslt file-paths.
   This is either done via the web API (not implemented yet) or CLI.
2. The `main.py` script opens a local socket connection with the daemon process.
3. The request is passed in a tuple with the following format: `(request-id#,input-filename.xml,input-filename.xsl)`.
4. The daemon process the arguments checking for errors.
5. The daemon process spawns a thread and calls the `transform_documents` method in that thread.
6. Once the thread completes the transofrmation it returns a status code which is propagated up to the `main.py` script and returns to the calling process.

# Application Daemon

In order to preserve state in the in-memory cache, XZES has a long-running daemon.

Here is roughly how that daemon works and how it recieves jobs.

```
 +----------------------+
 |                      |
 |Main Executable       |
 |                      |
 +-----+-----------^----+
       |           |
+-------------------------------------------------------+
|      |           |                                    |
|      |           |                                    |
|  +---v-----------+----+      +--------------------+   |
|  |                    +------>                    |   |
|  |Daemon Process      |      |Transformer         |   |
|  |                    <------+                    |   |
|  +--------------------+      +----+--------^------+   |
|                                   |        |          |
|                                   |        |          |
|  +--------------------------------v--------+------+   |
|  |                                                |   |
|  |Cache                                           |   |
|  |                                                |   |
|  +------------------------------------------------+   |
|                                                       |
+-------------------------------------------------------+
```

## The Daemon process startup

1. When the `daemon` binary is executed it sets up a long running process and goes into a state of cybernation.
2. Before exiting it:
  - Opens and listens on a socket `/var/run/xzes40daemon.socket`.
  - Allocates a block of memory for the cache.

## Spawning a Transformation

1. When the `main` binary is executed it is passed two or three files (input xml and xslt file-paths, with an optional output file path).
2. The main process opens a local socket connection with the daemon process by reading the `daemon.pid` file and connecting to that process.
3. The arguments passed to main are passed to the daemon process.
4. The daemon process reads in the arguments.
5. The daemon process spawns a thread and calls the `transform_documents` method int hat thread.
6. Once the thread completes it returns a status code which is propagated up through main and returned to the calling process.

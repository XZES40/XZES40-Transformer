# Parallel Computation

Parallel computation is how XZES40-transformer hold the faster transformer performance.

Parallel computation will hold multiply threats for XZES40-transformer application.
We will using Unix-socket lock function to handle the time conflict when we have mutiply file at the same time.

Here is how Parallel computation run.

1. First we have CGI script receive request from the front side.
2. CGI script may receive mutiply request at the same time, but they only can write at one time.
3. Daemon receive request and have mutiply thread to transfer the document, but only can return
   a transfered file at one time for one request. 

---


## Here is a map how parallel looks like:


                         +------------------+
                         |  daemon running  |
    +--------+           |    multiply      |
    |        |  request1 |     thread       |
    |  CGI   |  request2 |                  |
    | script +----------->                  |
    |        |  request3 |                  |
    |        |           |                  |
    |        |           |                  |
    +---^----+           |                  |
        |                +--------+---------+
        |                         |
        |                         |
        |                         |
        |                         |
        |                         |
        +-------------------------+
                 return


---


## Here is the time slot represent how parallel looks like:

### Daemon:

Request 1 received ->> create thread ->> do transformer ->> lock socket ->> return file ->> unlock socket

Request 2 received ->> create thread ->> do transformer ->>   __wait,locked socket__    ->> lock socket ->> return file ->> unlock socket

### CGI script:

Request 1 send ->> lock socket ->> write to the daemon ->> unlock socket ->>receive return file

Request 2 send ->> __wait,locked socket__ ->> lock socket after detect unlock ->>  write to daemon ->> unlock socket ->> receive return file

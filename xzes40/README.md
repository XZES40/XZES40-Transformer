# XZES40 Project

These projects include:

- `transformer`

## Directory Structure

Each project is organized according to a ['better than nothing' standardized structure][cpp-project].
This looks as follows:

```
project/
|-- build
|   `-- generated binaries (*.o, main)
|-- doc
|   `-- documentation (*.txt, *.md, *.rst)
|-- examples
|   `-- code examples not necessary for building but helpful
|-- include
|   `-- headers (*.h, *.hpp)
|-- lib
|   `-- external dependencies (*.h, *.hpp, *.c, *.cpp)
|-- Makefile
|-- src
|   `-- code (*.c, *.cpp)
`-- test
    `-- unit tests (*.c, *.cpp)
```

[cpp-project]: http://stackoverflow.com/questions/10782554/how-to-organize-a-c-project#10782577

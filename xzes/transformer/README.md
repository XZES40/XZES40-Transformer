# Transformer

This is the C++ application which transforms and XML and XML style-sheet documents into an output XML document.
This transformation is requested over a local network connection on `localhost:40404` via an Apache CGI script found in `cgi-script` (located in the parent directory).

## Usage

### Building

Once you're in a VM with the correct packages installed (check the `vagrant` directory for a reference as to what those packages are), you can build the project.

```
(transformer/)$ make
[... lots of verbose output ...]
```

### CLI Usage

Now that you've built the project you can *use* it.
How fancy.

```
user@host:transformer/$ ./build/xzesd &    # This runs the daemon in a background process
[2] 14876
user@host:transformer/$ ./build/main.py `pwd`/examples/simple.xml `pwd`/examples/simple.xsl /tmp/example.xml
user@host:transformer/$ cat /tmp/example.xml
<?xml version="1.0" encoding="UTF-8"?><out>Hello</out>
```

## Development

To develop the application you will need

- A Debian Linux host (probably a virtual machine).
- The C++ dependencies listed in the `vagrant` directory in the `setup.sh` script.
- The ability to host a website locally.

### Using Vagrant

Vagrant us a convenient tool for using a portable Virtual Machine.
Our development environment depends heavily on Vagrant.
It automatically runs a setup script which installs the dependencies, builds the binary, installs apache, sets up the daemon service, and starts the webserver to be accessed on your local virtual machine.

```
(XZES40-Transformer/)$ cd vagrant
(XZES40-Transformer/vagrant)$ vagrant up
Bringing machine 'default' up with 'virtualbox' provider...
==> default: Checking if box 'debian/jessie64' is up to date..
[...]
==> default: Vanilla Debian box. See https://atlas.hashicorp.com/debian/ for help and bug reports
(transformer/vagrant)$ vagrant ssh
[...[
vagrant@jessie:~$ cd xzes/xzes/transformer
```

See the file `vagant/README.md` for more information about the Vagrant development environment.

Even if you are not using Vagrant, the README and setup script should help you start contributing if you want to do that.

## Structure

This project is organized according to a ['better than nothing' standardized structure][cpp-project].
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

In addition to this structure, each source code file has

- A `.hpp` file in the `include` directory which outlines the classes, methods, and functions for that component.
- A `.cpp` file in the `src` directory which implements the classes, methods, and functions in that component.
- A `.md` file in the `doc` directory whichcovers the design, context, and oddities of that component.

All components in the project are compiled into `.o` files in the `build/` directory.
The entrypoint binary is compiled to `build/main`.

## Documentation

Documentation about the code is written in the `doc` directory but the source files are also self-documented.
For reference the `doc` directory tends to talk about the high-level concepts (what a compent is supposed to achieve and how it fits in) while the code documentation is targeted at helping developers find bugs and implement features.

[cpp-project]: http://stackoverflow.com/questions/10782554/how-to-organize-a-c-project#10782577

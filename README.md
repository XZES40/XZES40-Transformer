# XZES40-Transformer

*This project contains the Code, Code Documentation, Packaging, Tests, Examples, etc related to the Oregon State University + Apache Software Foundation Computer Science 2017 Capstone project.*

## Build status

- Linux: [![Build Status](https://travis-ci.org/XZES40/XZES40-Transformer.svg?branch=master)](https://travis-ci.org/XZES40/XZES40-Transformer)

## About the project

This project's purpose is to create an XML/XSLT transformation server which is

- *Fast*
- *Cross-platform*
- *Packaged for your OS*
- *Open Source*

It achieves these by:

- **IMPLEMENTED** Processing documents in parallel.
- **IMPLEMENTED** Using an in-memory cache to store processed documents.
- **NOT IMPLEMENTED** Using [FPM][fpm] to package the application.
- **IMPLEMENTED** Using the [Apache Webserver][apache] to handle requests.
- **IMPLEMENTED** And of course developing in the Open!

For a complete guide about the *design* and *purpose* of this project refer to our [LaTeX Documents Repository][capstone-repo].

## How do I use this?

The easiest way to use the application is to run a small Virtual Machine using Vagrant.
The application is designed to be run on Apache, but it is not encouraged that you install, run, or develop the application on your personal machine.

Vagrant is a CLI interface for pulling, provisioning, and running Virtual Machines with arbitrary backends; think VirtualBox or VMWare.
It runs on Linux, BSD, Windows, and MacOS.

Once you have Vagrant installed and setup on your system you should be able to run `vagrant up`.
Once the machine is setup it will give you a URL to visit in your local web browser.
Visit that page and you'll be using the application!

For information on running the application with Vagrant, see the `vagrant` directory's README file.

## Where is everything?

The code is contained in the `xzes` directory.
In that directory we have the following project components:

- `transformer` is the part of the project that acutally performs XML transformations.
- `cgi-glue` is the bridge between the transformer and web UI.
- `frontend` is the web interface for the application to request XML transformations.

Each of these directories has a README of it's own explaining what it is and how it works.


## How can I test?

In the directory `xzes/transformer/test/simple_test_file` we have provided a simple test script that you can run on your local system which connects to the server running via Vagrant..
Just run `./simple_test.sh` and then you should see HTTP 200 OK responses and transformed XML documents..

## Contributing

If you have interest in contributing to this project, **great**!

Please create an [issue][issues-url].

We have a [Vagrant Box][vagrant-code] which is probably your best way to test code-changes.

## A note about names

There might be a bit of confusion about what is named what and why:

- The project is called XZES40-Transformer because the Capstone group name is XZES40 (a combination of our names, our group number, and the letter X).
- The application is just called `xzes` because programs shouldn't have numbers in their name.
- The transformer daemon is called `xzesd`, and it's systemd service is called `xzesd.service.`
- The CGI script used by apache is called `xzes.py`.

So *most* things, unless they are the group or project, are just `xzes`.
The project as a whole is `XZES40-transformer`.
There are no components called simply `XZES40`.

## License

This software is licensed under `Apache 2.0`. Read the `LICENSE` file for more information.

[capstone-repo]: /xzes/cs-capstone-project
[issues-url]: /XZES40/XZES40-Transformer/issues
[fpm]: /jordansissel/fpm
[apache]: https://httpd.apache.org/
[vagrant-code]: vagrant/

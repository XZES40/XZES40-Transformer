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

- Processing documents in parallel.
- Using an in-memory cache to store processed documents.
- Using [FPM][fpm] to package the application.
- Using the [Apache Webserver][apache] to handle requests.
- And of course developing in the Open!

For a complete guide about the *design* and *purpose* of this project refer to our [Course LaTeX Documents Repository][capstone-repo].

## How do I use this?

This software is in the pre-pre-pre-unusable-Alpha stages right now so there's not much to *use*.
As the project develops further build and usage instructions will develop.

## Where is everything?

The code is contained in the `xzes40` directory.
In that directory we have the following projects:

- `transformer` is the C++ application which actually processes XML and XSLT documents.
- `cgi-glue` (pending) is the bridge between the backend (`transformer`) and frontend (`frontend`).
- `frontend` (pending) is the frontend application used to access the backend application.

Each of these directories has a README of it's own explaining what it is and how it works.

## Contributing

If you have interest in contributing to this project, **great**!

Please create an [issue][issues-url].

We have a [Vagrant Box][vagrant-code] which is probably your best way to test code-changes.

## License

This software is licensed under `Apache 2.0`. Read the `LICENSE` file for more information.

[capstone-repo]: /xzes40/cs-capstone-project
[issues-url]: /XZES40/XZES40-Transformer/issues
[fpm]: /jordansissel/fpm
[apache]: https://httpd.apache.org/
[vagrant-code]: vagrant/

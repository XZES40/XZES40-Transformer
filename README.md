# XZES40-Transformer

*This repository contains the Code, Code Documentation, Tests, Examples, and Build/Dev infrastructure for the Oregon State University + Apache Software Foundation Computer Science 2017 Capstone project.*
For more information about this project, see the project's [LaTeX document repository][capstone-repo]

## Build status

- Linux: [![Build Status](https://travis-ci.org/XZES40/XZES40-Transformer.svg?branch=master)](https://travis-ci.org/XZES40/XZES40-Transformer)

## About the project

This project's purpose is to create an XML/XSLT transformation server which is

- *Fast*
- *Cross-platform*
- *Packaged for your OS*
- *Open Source*

It achieves these by:

| Feature                                                   | Status              |
| --------------------------------------------------------- | ------------------- |
| Processing documents in parallel.                         | **IMPLEMENTED**     |
| Using an in-memory cache to store processed documents.    | **IMPLEMENTED**     |
| Using [FPM][fpm] to package the application.              | **NOT IMPLEMENTED** |
| Using the [Apache Webserver][apache] to handle requests.  | **IMPLEMENTED**     |
| And of course developing in the Open!                     | **IMPLEMENTED**     |

For a complete guide on the *design* and *purpose* of this project refer to our [LaTeX Documents Repository][capstone-repo].

## A what transformation server?

XML is a standard format for storing data, like JSON or YAML.
XSLT, or XML Style Sheets, are used for performing transformations on data in XML sheets.
For example:

### Problem

- You have an XML spreadsheet with information about all cars sold at a used car lot.
- You want to know how many *red* cars were sold between 2000 and 2015.

### Solution

Create an XML stylesheet (XSLT document) which specifies "if the <color> is red, and the <date sold> is between 2000 and 2015, include it in the output" (in psuedo XSLT).

The XZES40-Transformer application is used to perform these transformations over the internet so one can process data without installing heavy XML transformation applications locally.

## How do I use this?

The easiest way to use the application is to run a small Virtual Machine (VM).
The application is designed to be run on Apache, but it is not encouraged that you install, run, or develop the application on your personal machine.

The tool the development team uses to run the application in a VM is Vagrant.
Vagrant is a Command Line Interafce (CLI) interface for pulling, provisioning, and running VM with arbitrary back-ends.
This means you can use the same commands to run a VirtualBox, VMWare, or Docker machine.
Vagrant runs on Linux, BSD, Windows, and MacOS, so it is highly accessible for development.

Once you have Vagrant installed and setup on your system you should be able to run `vagrant up`.
Once the machine is setup it will give you a URL to visit in your local web browser.
Visit that page and you'll be using the application!

For information on running the application with Vagrant, see the [`vagrant` directory's README file][vagrant-code].

## Where is everything?

There are three components to the XZES40-Transformer project.

| Component     | Description                                                 |
| ------------- | ----------------------------------------------------------- |
| `transformer` | Core of the project. Actually performs XML transformations. |
| `cgi-glue`    | Bridge between the transformer and web UI.                  |
| `frontend`    | Web interface for the application. Accepts XML transformation requests, displays results to user. |

Each of these components can be found under the `xzes` directory.
Each component has an explanatory README which explains what it in greater detail is and how it works.


## How can I test?

In the directory `xzes/transformer/test/simple_test_file` we have provided a simple test script that you can run on your local system which connects to the server running via Vagrant..
Just run `./simple_test.sh` and then you should see HTTP 200 OK responses and transformed XML documents..

## Contributing

If you have interest in contributing to this project, **great**!

Please create an [issue][issues-url] and see if the developers respond.
If you have a fix for your issue, make a pull request by forking the project, doing your work on your fork, and clicking the `Create a Pull Request` button once you push your changes to that fork.

We have a [Vagrant Box][vagrant-code] which is probably your best way to test code-changes.
See the `vagrant` directory for details about that work-flow.

## A note about names

There might be a bit of confusion about what is named what and why:

- The project is called XZES40-Transformer because the Capstone group name is XZES40 (a combination of our names, our group number, and the letter X).
- The application is just called `xzes` because programs shouldn't have numbers in their name.
- The transformer daemon is called `xzesd`, and it's systemd service is called `xzesd.service.`
- The CGI script used by apache is called `xzes.py`.
- The component that runs between Apaache and the daemon is `cgi-glue` because it's just enough code to get `xzesd` exposed to the internet.
- The frontend is called `frontend` because that is an intuitive name.

*Most* things, unless they are the group or project, are just `xzes`.
The project as a whole is `XZES40-transformer`.
There are no components called simply `XZES40`, although during development some components *were*.

## License

This software is licensed under `Apache 2.0`. Read the `LICENSE` file for more information.

[capstone-repo]: /XZES40/cs-capstone-project
[issues-url]: /XZES40/XZES40-Transformer/issues
[fpm]: /jordansissel/fpm
[apache]: https://httpd.apache.org/
[vagrant-code]: vagrant/

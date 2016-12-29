# Vagrant development environment

This development environment requires the installation of [Vagrant][vagrant]
(and probably [VirtualBox][vbox]).

## About the dev environment

The Vagrant box includes the following:

- Creates a Debian Jessie virtual machine.
- Mounts the repo to the `/xzes40` directory (which is symlinked to `/home/vagrant/xzes40`).
- Installs the `xerces` and `xalan` libraries.

This is designed to the a minimal virtualbox only consisting of what is
*necessary* for the XZES40 application to run.
Nothing more, nothing less.

## Usage

1. Install Vagrant and a virtual machine runner (VirtualBox for instance).
2. In a shell nagivate to this directory.
3. Run `vagrant up` to start the virtual machine.
   If everything works correctly this command will output debug information and exit successfully.
   **READ THE OUTPUT OF THIS COMMAND**
4. Run `vagrant ssh` to login to the virtual machine.
5. On your host OS, run `vagrant suspend` to put the machine in hybernation.
6. On your host OS, run `vagrant destroy` to destroy the machine.
   This will not delete any data on your host machine, only data stored exclusively on the machine.

[vbox]: https://www.virtualbox.org/wiki/Downloads
[vagrant]: https://www.vagrantup.com/docs/getting-started/

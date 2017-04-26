# Vagrant development environment

This development environment requires the installation of [Vagrant][vagrant]
(and probably [VirtualBox][vbox]).

## About the dev environment

The Vagrant box includes the following:

- Creates a Debian Jessie virtual machine.
- Mounts the repo to the `/xzes` directory (which is symlinked to `/home/vagrant/xzes`).
- Installs the `xerces` and `xalan` libraries.

This is designed to a minimal virtualbox only consisting of what is
*necessary* for the XZES application to run.
Nothing more, nothing less.

## Usage

1. Install Vagrant and a virtual machine runner (VirtualBox for instance).
2. In a shell navigate to this directory.
3. Run `vagrant up` to start the virtual machine.
   If everything works correctly this command will output debug information and exit successfully.
   **READ THE OUTPUT OF THIS COMMAND**
4. Run `vagrant ssh` to login to the virtual machine.
5. On your host OS, run `vagrant suspend` to put the machine in hibernation.
6. On your host OS, run `vagrant destroy` to destroy the machine.
   This will not delete any data on your host machine, only data stored exclusively on the machine.

**NOTE:** You may need to install the vagrant plugin `vagrant-vbguest` if you get an error about mounting directories:

```
$ vagrant plugin install vagrant-vbguest
```

## About the setup script

The setup script in this directory is used to directly provision a stock Debian box.
If you intend to deploy this application, start with that script and work from there.
it does assume you have a checkout of this application at the directory `/xzes`, but with limited knowledge of bash and Linux systems one can appropriate the script for their own installation needs.

[vbox]: https://www.virtualbox.org/wiki/Downloads
[vagrant]: https://www.vagrantup.com/docs/getting-started/

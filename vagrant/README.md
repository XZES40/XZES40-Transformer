# Vagrant development environment

This development environment requires the installation of [Vagrant][vagrant]
(and probably [VirtualBox][vbox]).

## About the dev environment

The Vagrant box includes the following:

- Creates a Debian Jessie virtual machine.
- Mounts the repo to the `/xzes` directory (which is symlinked to `/home/vagrant/xzes`).
- Installs the `xerces` and `xalan` libraries.
- Sets up the Apache `xzes.py` script and VirtualHost.
- Enables and starts the `xzesd` systemd service.

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

Once the vagrant machine is up, i.e., just running `vagrant up` successfully, you should be able to go to `http://192.168.33.22:8000` in your web-browser and use an instance of the application for development.

### Debugging

If you are using this to develop the application, you should use `vagrant ssh` to login to the machine and run the following commands to figure out what is/is not working:

```
Is xzesd running?
$ sudo systemctl status xzesd
Is apache running?
$ sudo systemctl status apache2
What is the output of xzesd? (logs)
$ sudo journalctl -xe
Print all apache logs as they are generated.
Go to your browser and try using the website.
Only works if Apache is running (above)
$ sudo tail -f /var/log/apache2/*.log
```

## About the setup script

The setup script in this directory is used to directly provision a stock Debian box.
If you intend to deploy this application, start with that script and work from there.
If you are on a Debian system and have checked out this code repository, you should be able to use the `setup.sh` script with limited changes to install the application from source.

### Variables

The following scripts should be edited in the `setup.sh` script to correspond with the appropriate change in behavior.

```bash
# Ideally used in automation...
${TRAVIS_BUILD_DIR:="/xzes"}
# Specifies the directory where the repository is checked out
${XZES_SRC_DIR:=$TRAVIS_BUILD_DIR}
# Specifies where to install the application to
XZES_BIN="/usr/local/bin/"
# Specifies where to push cgi scripts
XZES_WWW_BIN="/var/www/cgi-bin"
# Where to put html/css/js files
XZES_WWW="/var/www/xzes"
# If set to true, copies files instead of sym-linking them
XZES_INSTALL=false
# set this to false if you do not want a symlink to the code in your homedirectory
# otherwise set it to your user name
DEV_USER="vagrant"
```

The most important would be `XZES_INSTALL` which is a boolean to either install or not install the application, and `XZES_SRC_DIR` which specifices where the checkout of XZES40-Transformer is.

[vbox]: https://www.virtualbox.org/wiki/Downloads
[vagrant]: https://www.vagrantup.com/docs/getting-started/

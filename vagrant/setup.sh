#!/bin/bash
+x

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

apt update -y
apt install -y curl \
               build-essential \
               libxalan-c-dev \
               libxalan-c-doc \
               libxalan-c111 \
               xalan \
               libxerces-c-dev \
               libxerces-c-doc \
               libxerces-c-samples \
               libxerces-c3.1 \
               clang \
               apache2 \
			   vim

# Ensure all directories exist that ought to 
mkdir --parents $XZES_WWW_BIN
mkdir --parents $XZES_BIN

if $DEV_USER; then
    ln -sf $XZES_SRC_DIR /home/$DEV_USER/xzes
fi

if ! which apache2ctl > /dev/null || ! [ -d $XZES_SRC_DIR ] ; then
    echo Please install apache2 or clone the XZES40-Transformer repository.
    exit 1
fi


# Build the daemon and copy it to the bin
cd $XZES_SRC_DIR/xzes/transformer/
make
if $XZES_INSTALL; then
    cp -rf $XZES_SRC_DIR/xzes/transformer/build/xzesd $XZES_BIN/xzesd
else
    ln -sf $XZES_SRC_DIR/xzes/transformer/build/xzesd $XZES_BIN/xzesd
fi

# Copy the cgi script to the correct location
mkdir -p /var/www/cgi-bin/
if $XZES_INSTALL; then
    cp -rf $XZES_SRC_DIR/xzes/cgi-glue/xzes.py $XZES_WWW_BIN/xzes.py
else
    ln -sf $XZES_SRC_DIR/xzes/cgi-glue/xzes.py $XZES_WWW_BIN/xzes.py
fi
chown -R www-data:www-data $XZES_WWW_BIN

# Copy the apache config file to the correct location
if $XZES_INSTALL; then
    cp -rf $XZES_SRC_DIR/xzes/xzes.conf /etc/apache2/sites-available/
else
    ln -sf $XZES_SRC_DIR/xzes/xzes.conf /etc/apache2/sites-available/
fi
ln -sf /etc/apache2/sites-available/xzes.conf /etc/apache2/sites-enabled/
chown -R www-data:www-data /etc/apache2/sites-{enabled,available}

# Copy the frontend interface
rm -rf $XZES_WWW
if  $XZES_INSTALL; then
    cp -rf $XZES_SRC_DIR/xzes/frontend $XZES_WWW
else
    ln -sf $XZES_SRC_DIR/xzes/frontend $XZES_WWW
fi
chown -R www-data:www-data $XZES_WWW

# Restart Apache
a2enmod cgi
sleep 5
systemctl restart apache2

# Copy the systemd file to the correct location and start the daemon
if $XZES_INSTALL; then
    cp -rf $XZES_SRC_DIR/xzes/xzesd.service /etc/systemd/user/xzesd.service
    systemctl enable --force /etc/systemd/user/xzesd.service
else
    systemctl enable --force $XZES_SRC_DIR/xzes/xzesd.service
fi
systemctl daemon-reload
systemctl start xzesd.service

echo "===================================================================="
echo "Visit http://192.168.33.22:8000/xzes/ to view the webiste in action!"
echo "===================================================================="

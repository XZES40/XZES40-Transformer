###
# Description: Setup for the XZES40 application.
# Pre-requisites:
# - A checkout of this respository at $XZES40_SRC
# - Apache2
###

XZES40_SRC="/xzes40/"
XZES40_BIN="/usr/local/bin/"

+x

if ! which apache2ctl > /dev/null || ! [ -d $XZES40_SRC ] ; then
    echo Please install apache2 or clone the XZES40-Transformer repository.
    exit 1
fi

mkdir --parents $XZES40_BIN

# Build the daemon and copy it to the bin
cd $XZES40_SRC/xzes40/transformer/
make
ln -sf $XZES40_SRC/xzes40/transformer/build/daemon $XZES40_BIN/xzes-daemon

# Copy the cgi script to the correct location
mkdir -p /var/www/cgi-bin/
ln -sf $XZES40_SRC/xzes40/cgi-glue/xzes.py /var/www/cgi-bin/xzes.py
chown -R www-data:www-data /var/www

# Copy the apache config file to the correct location
ln -sf $XZES40_SRC/xzes40/xzes40.conf /etc/apache2/sites-available/
ln -sf /etc/apache2/sites-available/xzes40.conf /etc/apache2/sites-enabled/
chown -R www-data:www-data /etc/apache2/sites-{enabled,available}

# Copy the frontend interface
ln -sf $XZES40_SRC/xzes40/frontend /var/www/xzes40

# Restart Apache
a2enmod cgi
sleep 5
systemctl restart apache2

# Copy the systemd file to the correct location and start the daemon
systemctl enable --force $XZES40_SRC/xzes40/xzes40.service
systemctl daemon-reload
systemctl start xzes40.service

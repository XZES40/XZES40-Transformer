#!/bin/bash

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
               apache2

ln -sf /xzes40 /home/vagrant/xzes40

# Configure the webserver
## Setup apache conf file
cp /xzes40/xzes40/cgi-glue/xzes40-cgi.conf /etc/apache2/sites-available/
ln -sf /etc/apache2/sites-available/xzes40-cgi.conf /etc/apache2/sites-enabled/
chown -R www-data:www-data /etc/apache2/sites-{enabled,available}
## Place script
mkdir -p /var/www/cgi-bin/
ln -sf /xzes40/xzes40/cgi-glue/xzes.py /var/www/cgi-bin/xzes.py
sudo chown -R www-data:www-data /var/www
## Restart Apache
a2enmod cgi
systemctl restart apache2

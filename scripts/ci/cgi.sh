#!/bin/bash

${TRAVIS_BUILD_DIR:=/xzes40}

## Setup apache conf file
cp $TRAVIS_BUILD_DIR/xzes40/cgi-glue/xzes40-cgi.conf /etc/apache2/sites-available/
ln -sf /etc/apache2/sites-available/xzes40-cgi.conf /etc/apache2/sites-enabled/
chown -R www-data:www-data /etc/apache2/sites-{enabled,available}

## Place script
mkdir -p /var/www/cgi-bin/
ln -sf $TRAVIS_BUILD_DIR/xzes40/cgi-glue/xzes.py /var/www/cgi-bin/xzes.py
sudo chown -R www-data:www-data /var/www

## Restart Apache
a2enmod cgi

sleep 5

systemctl restart apache2

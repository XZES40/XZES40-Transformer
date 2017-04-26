#!/bin/bash

${TRAVIS_BUILD_DIR:=/xzes}

## Setup apache conf file
cp $TRAVIS_BUILD_DIR/xzes/xzes.conf /etc/apache2/sites-available/
ln -sf /etc/apache2/sites-available/xzes.conf /etc/apache2/sites-enabled/
chown -R www-data:www-data /etc/apache2/sites-{enabled,available}

## Place script
mkdir -p /var/www/cgi-bin/
ln -sf $TRAVIS_BUILD_DIR/xzes/cgi-glue/xzes.py /var/www/cgi-bin/xzes.py
chown -R www-data:www-data /var/www

## Restart Apache
a2enmod cgi

sleep 5

systemctl restart apache2

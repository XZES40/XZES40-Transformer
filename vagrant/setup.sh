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
               apache2 \

ln -s /xzes40 /home/vagrant/xzes40

# Configure the webserver
cp /xzes40/xzes40/cgi-glue/xzes40-cgi.conf /etc/apache2/sites-avaliable
ln -s /etc/apache2/sites-avaliable /etc/apache2/sites-enabled
a2enmod cgi
systemctl restart apache

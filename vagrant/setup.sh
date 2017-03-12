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
               libapache2-mod-fcgid \
               pyhton-fcgi

ln -s /xzes40 /home/vagrant/xzes40

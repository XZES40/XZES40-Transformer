#!/bin/bash

# if $TRAVIS_BUILD_DIR is unset, set it (for local vagrant stuff)
${TRAVIS_BUILD_DIR:=/xzes}

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
/xzes40/scripts/setup.sh

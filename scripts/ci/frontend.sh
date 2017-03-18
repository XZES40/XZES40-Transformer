#!/bin/bash

${TRAVIS_BUILD_DIR:=/xzes40}


rm -rf /var/www/xzes40

cp -r $TRAVIS_BUILD_DIR/xzes40/frontend /var/www/xzes40

chown -R www-data:www-data /var/www/*

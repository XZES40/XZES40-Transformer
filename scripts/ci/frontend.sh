#!/bin/bash

${TRAVIS_BUILD_DIR:=/xzes}


rm -rf /var/www/xzes

cp -r $TRAVIS_BUILD_DIR/xzes/frontend /var/www/xzes

chown -R www-data:www-data /var/www/*

#!/bin/bash

${TRAVIS_BUILD_DIR:=/xzes40}

sudo cp -r $TRAVIS_BUILD_DIR/xzes40/frontend /var/www/html/xzes

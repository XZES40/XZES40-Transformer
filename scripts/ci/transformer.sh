#!/bin/bash
#
# Transformer CI Script
#
# Tasks:
# - Runs `make` to ensure software builds
# - Runs tests (ish)
#

${TRAVIS_BUILD_DIR:=/xzes}

# Navigate to the transformer software directory
cd $TRAVIS_BUILD_DIR/xzes/transformer

# Run the make instructions
make

# Test that the CGI script works
# $TRAVIS_BUILD_DIR/scripts/ci/cgi.sh

# Start the daemon
# $TRAVIS_BUILD_DIR/xzes/transformer/build/daemon &

# Run the cgi test script
# $TRAVIS_BUILD_DIR/xzes/cgi-glue/simple_test.sh

#!/bin/bash
#
# Transformer CI Script
#
# Tasks:
# - Runs `make` to ensure software builds
#
# Future tasks:
# - Runs tests
#

# Navigate to the transformer software directory
cd $TRAVIS_BUILD_DIR/xzes40/transformer

# Run the make instructions
make

# TODO: Add more checks and tests

#!/bin/bash

${TRAVIS_BUILD_DIR:=/xzes40}

# Navigate to the transformer software directory
cd $TRAVIS_BUILD_DIR/xzes40/transformer

# Run the make instructions
make

# Start the daemon
$TRAVIS_BUILD_DIR/xzes40/transformer/build/xzes40d &

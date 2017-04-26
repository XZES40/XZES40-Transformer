#!/bin/bash

${TRAVIS_BUILD_DIR:=/xzes}

# Navigate to the transformer software directory
cd $TRAVIS_BUILD_DIR/xzes/transformer

# Run the make instructions
make

# Start the daemon
$TRAVIS_BUILD_DIR/xzes/transformer/build/xzesd &

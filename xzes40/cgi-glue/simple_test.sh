#!/bin/bash

if [ ! -S /tmp/xzes40daemon.socket ]; then
    /home/vagrant/xzes40/xzes40/transformer/build/daemon &
fi

sudo chown www-data:www-data /tmp/xzes40daemon.socket

curl \
  --include \
  --request POST \
  --header "Content-Type: multipart/form-data" \
  --form "xml=@/home/vagrant/xzes40/xzes40/transformer/examples/simple.xml" \
  --form "xsl=@/home/vagrant/xzes40/xzes40/transformer/examples/simple.xsl" \
  localhost:8080/xzes.py

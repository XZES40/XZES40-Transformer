#!/bin/bash

curl \
  --include \
  --request POST \
  --header "Content-Type: multipart/form-data" \
  --form "xml=@$1" \
  --form "xsl=@$2" \
  http://192.168.33.22:8080/cgi-bin/xzes.py

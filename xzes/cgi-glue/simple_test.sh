#!/bin/bash

XZES_HOST=${XZES_HOST:-192.168.33.22}
XZES_PORT=${XZES_PORT:-8080}
XZES_PATH=${XZES_PATH:-cgi-bin/xzes.py}

if [ $# -eq 0 ];
    then
        echo "USAGE:"
        printf "\t./simple_test.sh /path/to/somefile.xml /path/to/somefile.xsl"
        echo "OPTIONALLY:"
        printf "\tXZES_HOST=192.168.33.22 XZES_PORT=8080 XZES_PATH=cgi-bin/xzes.py ./simple_test.sh /path/to/somefile.xml /path/to/somefile.xsl"
else
    curl \
      --include \
      --request POST \
      --header "Content-Type: multipart/form-data" \
      --form "xml=@$1" \
      --form "xsl=@$2" \
      --form "parameters={}" \
      http://$XZES_HOST:$XZES_PORT/$XZES_PATH
fi

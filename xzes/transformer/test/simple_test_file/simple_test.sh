#!/bin/bash

XZES_HOST=${XZES_HOST:-192.168.33.22}
XZES_PORT=${XZES_PORT:-8000}
XZES_PATH=${XZES_PATH:-cgi-bin/xzes.py}

echo "========================================"
echo "Start testing"
echo "1st test simple file without parameter passing"
echo "========================================"

curl \
	--include \
    --request POST \
    --header "Content-Type: multipart/form-data" \
    --form "xml=@sample1.xml" \
    --form "xsl=@sample1.xsl" \
    --form "parameters={}" \
    http://$XZES_HOST:$XZES_PORT/$XZES_PATH

echo "========================================"
echo "2nd test file with two parameter passing"
echo "========================================"
curl \
	--include \
    --request POST \
    --header "Content-Type: multipart/form-data" \
    --form "xml=@sample2.xml" \
    --form "xsl=@sample2.xsl" \
    --form "parameters={ \"param1\":\"val1\",\"param2\":\"val2\" }" \
    http://$XZES_HOST:$XZES_PORT/$XZES_PATH

echo "========================================"
echo "3rd test file with five parameter passing"
echo "========================================"
curl \
	--include \
    --request POST \
    --header "Content-Type: multipart/form-data" \
    --form "xml=@sample3.xml" \
    --form "xsl=@sample3.xsl" \
    --form "parameters={ \"param1\":\"val1\",\"param2\":\"val2\",\"param3\":\"val3\",\"param4\":\"val4\",\"param5\":\"val5\"  }" \
    http://$XZES_HOST:$XZES_PORT/$XZES_PATH

echo "========================================"
echo "4th test file with four parmeter passing"
echo "parameter 4 should print defult value"
echo "========================================"
curl \
	--include \
    --request POST \
    --header "Content-Type: multipart/form-data" \
    --form "xml=@sample3.xml" \
    --form "xsl=@sample3.xsl" \
    --form "parameters={ \"param1\":\"val1\",\"param3\":\"val3\",\"param4\":\"val4\",\"param5\":\"val5\"  }" \
    http://$XZES_HOST:$XZES_PORT/$XZES_PATH

echo "========================================"
echo "5th test for checking cache store."
echo "We post sample1 file again, and test if there is file exist in the cache."
echo "We have go to system log to check if this work."
echo "========================================"
curl \
	--include \
    --request POST \
    --header "Content-Type: multipart/form-data" \
    --form "xml=@sample1.xml" \
    --form "xsl=@sample1.xsl" \
    --form "parameters={}" \
    http://$XZES_HOST:$XZES_PORT/$XZES_PATH



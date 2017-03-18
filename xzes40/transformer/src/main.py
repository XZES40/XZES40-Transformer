#!/usr/bin/env python2
###############################################################################
## Copyright 2017, the Oregon State University Capstone Project 'XZES40'
## with the Apache Software Foundation
##
## Licensed under the Apache License, Version 2.0 (the "License");
## you may not use this file except in compliance with the License.
## You may obtain a copy of the License at
##
## http://www.apache.org/licenses/LICENSE-2.0
##
## Unless required by applicable law or agreed to in writing, software
## distributed under the License is distributed on an "AS IS" BASIS,
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
## See the License for the specific language governing permissions and
## limitations under the License.
###############################################################################

###############################################################################
## Filename: xzes40.cpp
## Description: Main entrypoint for the XZES40 Transformer application
###############################################################################

import socket
import sys
import os

def main():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(("localhost", 40404))

    x = parse_args(sys.argv)
    if "" != x:
        # print("XZES:: Sent request:", x)
        s.send(x.encode('utf-8'))
    else:
        print("\nPlease pass an XML and XSL file in the following format:")
        print("\tmain.py document.xml stylesheet.xsl outfile.xml\n")

def parse_args(a):
    """
    Parses the arguments passed to main.py.
    Return "argv[1],argv[2]"
    """
    if len(a) == 4:
        return "{},{},{},{},{}".format(uid(a), a[1], a[2], a[3], "")
    else:
        return ""

def uid(a):
    """
    Returns a unique id.
    """
    return 1

if __name__ == '__main__':
    main()

#!/usr/bin/env python2
# -*- coding: UTF-8 -*-# enable debugging
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
## Filename: xzes.py
## Description: CGI glue between the XZES40 Daemon and the outside world.
###############################################################################
import cgi
import socket
import sys
import os
import cgitb; cgitb.enable() # for troubleshooting

XZES_SAVE_PATH = "/tmp/xzes"

response = """<p>
{}
</p>
"""

def main(r):
    socket_path = "/tmp/xzes40daemon.socket"
    print("Content-Type: application/xml; charset=utf-8\n\n")

    try:
        s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        s.connect(socket_path)
    except:
        print(r.format("Error connecting to socket {}.\nPermissions error?".format(socket_path)))

    try:
        form = cgi.FieldStorage()
        xml = ''.join(form['xml'].file.readlines())
        xsl = ''.join(form['xsl'].file.readlines())
    except:
        print(r.format("Error parsing form.\nDid you send the right form?"))
        return 1

    if not os.path.exists(XZES_SAVE_PATH):
        os.mkdir(XZES_SAVE_PATH)

    try:
        xml_path = save_file(xml, '.xml')
        xsl_path = save_file(xsl, '.xsl')
    except:
        print(r.format("Error saving POST files.\nPermissions error?"))
        return 1

    try:
        job = "{},{},{}".format(hash(xml+xsl), xml_path, xsl_path).encode("utf-8")
    except:
        print(r.format("Error generating job request.\n*shrug*"))

    try:
        s.send(job)
    except:
        print(r.format("Error requesting job"))
        return 1

    try:
        print(r.format(job))
    except:
        print(r.format("Error.\nI have no idea why..."))


def save_file(contents, ext=''):
    """
    saves `contents` to `XZES_SAVE_PATH/<uid(contents)><ext>`
    e.g., /tmp/xzes/1871149974711195521.out
    """
    uid = str(hash(contents))
    fpath = os.path.join(XZES_SAVE_PATH, uid + ext)
    if not os.path.isfile(fpath):
        with open(fpath, 'w') as f:
            f.write(contents)
    return fpath

if __name__ == '__main__':
    main(response)


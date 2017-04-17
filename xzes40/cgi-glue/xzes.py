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

response = """Content-Type: text/plain; charset=utf-8\n\n\n{}"""
error = """<?xml version="1.0" encoding="UTF-8"?><error>{}</error>"""

def main(r, e):
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect(("localhost", 40404))
    except:
        print(r.format(e.format("Error connecting to socket.\nPermissions error?")))

    try:
        form = cgi.FieldStorage()
        xml = ''.join(form['xml'].file.readlines())
        xsl = ''.join(form['xsl'].file.readlines())
    except:
        print(r.format(e.format("Error parsing form.\nDid you send the right form?")))
        return 1

    if not os.path.exists(XZES_SAVE_PATH):
        os.mkdir(XZES_SAVE_PATH)

    try:
        xml_path = save_file(xml, '.xml')
        xsl_path = save_file(xsl, '.xsl')
    except:
        print(r.format(e.format("Error saving POST files.\nPermissions error?")))
        return 1

    tmp_job_id = hash(xml+xsl)
    if tmp_job_id < 0:
        tmp_job_id *= -1
    job_id   = str(tmp_job_id)
    out_path = os.path.join(XZES_SAVE_PATH, job_id + '.xml')
    try:
        job = "{},{},{},{},{}".format(job_id  ,
                                      xml_path,
                                      xsl_path,
                                      out_path, "").encode("utf-8")
    except:
        print(r.format(e.format("Error generating job request.\n*shrug*")))
        return 1

    try:
        s.send(job)
    except:
        print(r.format(e.format("Error requesting job")))
        return 1

    # Format: "job_id,Maybe /path/to/output.xml,Maybe error"
    data = s.recv(2048)

    try:
        split = data.split(',')
    except:
        print(r.format(e.format("Response from daemon was malformed")))
        s.close()
        return 1

    try:
        if split[2] == "":
            with open(split[1], 'r') as f:
                print(r.format(''.join(f.readlines())))
                s.close()
                return 1
        else:
            print(r.format(e.format(split[2])))
            s.close()
            return 1
    except:
        print(r.format(e.format("Something went wrong!: [{}] [{}] [{}]".format(data, job_id, out_path))))
        s.close()
        return 1


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
    main(response, error)


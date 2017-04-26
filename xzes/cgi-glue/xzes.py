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
## Description: CGI glue between the XZES Daemon and the outside world.
###############################################################################
import cgi
import socket
import traceback
import sys
import os
import json
import cgitb; cgitb.enable() # for troubleshooting

XZES_SAVE_PATH = "/tmp/xzes"

error = """Content-Type: text/plain; charsetutf-8\nStatus: 400 Bad Request\n\n{}"""
success = """Content-Type: application/xml; charset=utf-8\nStatus: 200 OK\n\n{}"""

def main(ok, bad):
    # Establish a connection with the local networked daemon
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        # TODO make this an environemnt variable, not hard coded.
        s.connect(("localhost", 40404))
    except:
        print(bad.format("Error connecting to socket.\nPermissions error?"))
        s.close()
        return 1

    # fetch the form provided by the CGI library
    try:
        form = cgi.FieldStorage()
        xml = ''.join(form['xml'].file.readlines())
        xsl = ''.join(form['xsl'].file.readlines())
    except:
        print(bad.format("Error parsing form.\nDid you send the right form?"))
        s.close()
        return 1

    ## create the directory the files will be saved to (/tmp/xzes)
    if not os.path.exists(XZES_SAVE_PATH):
        os.mkdir(XZES_SAVE_PATH)

    ## Save a local copy of the xml files
    try:
        xml_path = save_file(xml, '.xml')
        xsl_path = save_file(xsl, '.xsl')
    except:
        print(bad.format("Error saving POST files.\nPermissions error?"))
        s.close()
        return 1

    # Parse the parameters.
    # Stringified JSON {key:val,...}
    try:
        parameters = json.loads(form['parameters'].value)
    except:
        print(bad.format("There was an error parsing your parameters"))
        return 1

    # Create a uniqe job-id for this request
    tmp_job_id = hash(xml+xsl+str(parameters))
    # Ensure it's positive
    if tmp_job_id < 0:
        tmp_job_id *= -1
    # Convert it to a string
    job_id   = str(tmp_job_id)
    # Set this to be the output path
    out_path = os.path.join(XZES_SAVE_PATH, job_id + '.xml')

    # Begin setting up the job.
    # job_id,local_xml_path,local_xsl_path,local_output_path
    try:
        job = "{},{},{},{}".format(job_id  ,
                                   xml_path,
                                   xsl_path,
                                   out_path).encode("utf-8")
    except:
        print(bad.format("Error generating job request.\n*shrug*"))
        s.close()
        return 1

    # Add any custom parameters to the job
    for k, v in parameters.iteritems():
        job += ",{},\'{}\'".format(k,v)

    # prevents weird parsing errors when tokenizing the stream.
    job += ","

    # Send the job over the local network connection
    try:
        s.send(job)
    except:
        print(bad.format("Error requesting job"))
        s.close()
        return 1

    # Format: "job_id,Maybe /path/to/output.xml,Maybe error"
    data = s.recv(2048)

    # Split the repsonse on '.'
    # expects job_id,local_ouptut_path,optional_error
    try:
        split = data.split(',')
    except:
        print(bad.format("Response from daemon was malformed"))
        s.close()
        return 1

    try:
        # If there is no error...
        if split[2] == "":
            # open the output file
            with open(split[1], 'r') as f:
                # send it to the user.
                print(ok.format('\n'.join(f.readlines())))
                s.close()
                return 1
        # Otherwise send the error propogated from the daemon.
        else:
            print(bad.format(split[2]))
            s.close()
            return 1
    except:
        print(bad.format("Something went wrong!"))
        s.close()
        return 1


def save_file(contents, ext=''):
    """
    saves `contents` to `XZES_SAVE_PATH/<uid(contents)><ext>`
    e.g., /tmp/xzes/1871149974711195521.out
    """
    # Create a uniq hash.
    # The user will never see this so uniqueness is the top priority
    uid = str(hash(contents))
    # Create the path to save the file
    fpath = os.path.join(XZES_SAVE_PATH, uid + ext)
    # If it's not already saved
    # Open the file and write the contents
    if not os.path.isfile(fpath):
        with open(fpath, 'w') as f:
            f.write(contents)
    # Return the path it was saved to
    return fpath

if __name__ == '__main__':
    sys.stderr = sys.stdout
    # If all else fails
    try:
        main(success, error)
    # Print a traceback
    except:
        print "\n\n<PRE>"
        traceback.print_exc()


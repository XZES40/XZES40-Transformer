# cgi-glue

This component of the project is a python CGI script which accepts HTTP requests via Apache, processes them into a format that the xzesd daemon can read, saves uploaded files, waits for the transformation to complete, and responds with the output text from the daemon.

`xzes.py` is the meat of it. Read the comments or more information.

`simple_test.sh` is used to test to see if the application is running correctly. Pass it an xml and xsl file (in that order) and see if it responds correctly.

`benchmarks.sh` was used to benchmark the speed of the application for grading purposes.

This is essentially treated by the frontend as a *very* simple text-based web API.
A form with two files (`xml`, `xsl`) and parameters (`{key: val, key2,val2}`) are accepted, processed, serialized, and sent to the daemon over a local network connection (`localhost:40404`).

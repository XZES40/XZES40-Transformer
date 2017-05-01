# Parse

The file `parse.cpp` contains functionality for parsing input recieved from the user via the python CGI script into a struct which is easy to operate on.

That input is in the following format:

`"job_id,xml_file_path,xsl_file_path,output_file_path,key1,val1,key2,val2,key3,..."`

This means that the daemon recieves what essentially amounts to a stream which is then parsed.

The `key1,val1,key2,val2` are tuples of key=val pairs passed to the transformer for custom tranfsormations.

#!/bin/bash

# Benchmarks our application
# Usage: Run this in the `xzes/cgi-glue/` directory.
#        Provide it a number as a uid.
#        Output data is stored in output.<uid>.time and output.<uid>.docs
#        Check which docs did not build correctly in output.<uid>.docs
#        Check the time it took in output.<uid>.time.

stylesheet="../../extra/xdocs/stylebook.xslt"

if [ $# -eq 0 ]; then
    echo "No arguments provided"
    echo "Please pass a job uid"
    exit 1
fi

for xmldoc in `find ../../extra/xdocs/*.xml`; do
    echo $xmldoc >> output.time
    echo $xmldoc >> output.docs
    { time sh ./simple_test.sh $xmldoc $stylesheet >> output.docs 2>&1 ; } 2>> output.time
done

mv output.time output.$1.time
mv output.docs output.$1.docs

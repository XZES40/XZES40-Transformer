#!/bin/bash
#
# This script is used to restore group "staff" permissions
# to everything in the /data/* directory trees. It also
# patches the file and directory modes in case the user
# or effective umask was 022 instead of the required 002.
#

chown root:staff /data
chmod 3777 /data
cd /data
find . -exec chgrp staff {} \;
find . -exec chmod g+w {} \;

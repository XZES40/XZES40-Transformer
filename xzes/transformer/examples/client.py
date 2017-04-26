# ------------------------------------------------------------------------------
# client.py: a proof of concept in using python to communicate with the daemon
# over unix sockets.
#
# Usage:
# 1. Begin `examples/daemon`
# 2. In another window run `python client.py some other arguments`
# 3. Notice how the daemon responds to new messages.
# ------------------------------------------------------------------------------
import socket
import sys
import os

socket_path = "/tmp/example.socket"

s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
s.connect(socket_path)

x = ' '.join(sys.argv)
if "" != x:
    print("SEND:", x)
    s.send(x.encode('utf-8'))

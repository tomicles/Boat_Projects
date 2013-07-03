#!/usr/bin/python

import socket
import fileinput

sock=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)

#ip=socket.gethostbyaddr("hackercomplex.dyndns.org")[2][0]
ip="107.3.154.121"
for line in fileinput.input():
    print "{data}".format(data=line)
    sock.sendto("{data}\n".format(data=line),(ip,18001))




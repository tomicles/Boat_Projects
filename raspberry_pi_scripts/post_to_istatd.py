#!/usr/bin/python

import sys
import time
import socket
import glob
import os
import subprocess as sp
from daemon import Daemon
from functools import partial

def get_ip():
    res = sp.call("iwconfig | grep Mifi-Santana2023", shell=True)
    return socket.gethostbyaddr("hackercomplex.dyndns.org")[2][2] if res == 1 else "192.168.24.239"

def send_to_istatd(counter, data, sock, dest):
    sock.sendto("{counter} {data}\n".format(counter=counter, data=data), dest)

def extract_data(file, send_fun):
    def read(file):
        with open(file, 'r') as f:
            return f.read()

    counter_path, _ = os.path.splitext(file)
    _, counter = os.path.split(counter_path)
    send_fun(counter=counter, data=read(file))

class IstatdDaemon(Daemon):
    def run(self):
        ip = get_ip()
        send_with_ip = partial(send_to_istatd, dest=(ip, 18001))
        while True:
            sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            send_with_socket = partial(send_with_ip, sock=sock)
            extract_from_file = partial(extract_data, send_fun=send_with_socket)

            map( extract_from_file, glob.glob('/var/tmp/data/*.out'))
            time.sleep(5)


def main(argv):
    myDaemon = IstatdDaemon('/tmp/istatd.pid')

    if 'start' == argv[1]:
        myDaemon.start()
    elif 'stop' == argv[1]:
        myDaemon.stop()
    elif 'restart' == argv[1]:
        myDaemon.restart()
    elif 'run' == argv[1]:
        myDaemon.run()
    else:
        print 'usage: {script} start|stop|restart|run'.format(script=argv[0])
        sys.exit(2)
    sys.exit(0)

if __name__ == "__main__":
    main(sys.argv)

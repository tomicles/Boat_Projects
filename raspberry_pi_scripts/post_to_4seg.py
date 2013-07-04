#!/usr/bin/python

import sys
import time
import socket
import glob
import os
import time
import string
import math
from daemon import Daemon
from functools import partial

import serial


def send_to_4seg(data, ser):
    ser.write('\x76')
    ser.write('\x79')
    ser.write('\x00')
    idx = string.find(data, '.')
    print 'raw', data
    if idx == -1 or idx > 3:
        four = data[:4]
        ser.write(four)
    elif idx == 0:
        four = data[1:5]
        ser.write(four)
    else: #idx <=3
        four = data[:idx]+data[idx+1:5]

        def idx_to_bin(index):
            return 2**(index-1)

        def bin_to_str(bin):
            print 'position', bin
            return '\\x0{b}'.format(b=bin)

        def write_dec(pos):
            ser.write('\x77')
            ser.write(pos)

        ser.write(four)
        write_dec(bin_to_str(idx_to_bin(idx)))


def maybe_extract_data(file, extract_fun):
    if (time.time() - os.path.getmtime(file)) < 30:
        extract_fun(file)

def extract_data(file, send_fun):
    def read(file):
        with open(file, 'r') as f:
            return f.read()

    send_fun(data=read(file))

class FourSegDaemon(Daemon):
    def run(self):
        ser = serial.Serial(port='/dev/ttyUSB1')
        send_with_dev = partial(send_to_4seg, ser=ser)
        while True:
            extract_from_file = partial(extract_data, send_fun=send_with_dev)
            maybe_extract_from_file = partial(maybe_extract_data, extract_fun=extract_from_file)

            maybe_extract_from_file('/var/tmp/data/gps.speed.out')
            time.sleep(5)


def main(argv):
    myDaemon = FourSegDaemon('/tmp/istatd.pid')

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

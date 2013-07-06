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
    def reset_position():
        ser.write('\x79')
        ser.write('\x00')

    def write_dec(pos):
        ser.write('\x77')
        ser.write(pos)

    def clear_decimal():
        write_dec('\x00')

    idx = string.find(data, '.')
    if idx == -1 or idx > 3:
        clear_decimal()
        reset_position()
        four = data[:4]
        ser.write(four)
    elif idx == 0:
        clear_decimal()
        reset_position()
        four = data[1:5]
        ser.write(four)
    elif idx == 1:
        four = ' {three}'.format(three=data[:idx]+data[idx+1:4])
        reset_position()

        ser.write(four)
        write_dec('\x02')

    else: #idx <=3
        # 0.011 1.345 10.23 123.3
        # 00.01  01.34  10.23  123.3
        four = data[:idx]+data[idx+1:5]

        def idx_to_binstr(bin):
            return {
            1: '\x01', 
            2: '\x02', 
            3: '\x04', 
            4: '\x08', 
            }.get(idx)

        reset_position()

        ser.write(four)
        write_dec(idx_to_binstr(idx))


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
        ser.write('\x76')

        send_with_dev = partial(send_to_4seg, ser=ser)
        while True:
            extract_from_file = partial(extract_data, send_fun=send_with_dev)
            maybe_extract_from_file = partial(maybe_extract_data, extract_fun=extract_from_file)

            maybe_extract_from_file('/var/tmp/data/gps.speed.out')
            time.sleep(.5)


def main(argv):
    myDaemon = FourSegDaemon('/tmp/4seg.pid')

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

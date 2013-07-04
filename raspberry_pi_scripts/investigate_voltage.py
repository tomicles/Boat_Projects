#!/usr/bin/python

import sys
import time
import socket
import glob
import os
import time
import subprocess as sp
from daemon import Daemon
from functools import partial

def extract_data(file, investigate_fun):
    def read(file):
        with open(file, 'r') as f:
            return f.read()

    investigate_fun(data=read(file))

def investigate_voltage(data):
    """ for 
    vin_true = 3.298
    vout_true = .779
    """

    vout = float(data)
    z1 = 3220
    z2 = 995
    vin = vout * (z1+z2) / z2
    print 'for vout ', vout, ' vin: ', vin

class InvestigateVoltageDaemon(Daemon):
    def run(self):
        while True:
            extract_from_file = partial(extract_data, investigate_fun=investigate_voltage)

            map( extract_from_file, glob.glob('/var/tmp/data/voltage.*.out'))
            time.sleep(5)


def main(argv):
    myDaemon = InvestigateVoltageDaemon('/tmp/investigate_voltage.pid')

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

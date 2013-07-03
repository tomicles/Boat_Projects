#!/usr/bin/python

import sys
import time
import os

from functools import partial
import itertools

from daemon import Daemon
from util import dataWriter

basedir=os.path.dirname(os.path.realpath(__file__))
sys.path.append(basedir + '/./lib/Adafruit_Libraries/Adafruit_ADS1x15')

from Adafruit_ADS1x15 import ADS1x15

ADS1015 = 0x00 #12 bit ADC

class VoltageDaemon(Daemon):
    def run(self):
        adc = ADS1x15(ic=ADS1015)
        while True:
            volts = map(lambda x: adc.readADCSingleEnded(x, 4096, 250) / 1000, [0,1,2,3])
            writer = partial(dataWriter, prefix='/var/tmp/data')
            map(lambda (name, volt): writer(name, volt), zip(['voltage.a{n}'.format(n=x) for x in [0,1,2,3]], volts))
            time.sleep(1)


def main(argv):
    myDaemon = VoltageDaemon('/tmp/voltage.pid')

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

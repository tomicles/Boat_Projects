#!/usr/bin/python

import sys
import time
import os

from functools import partial
import itertools
from optparse import OptionParser
import json

from daemon import Daemon
from util import dataWriter

basedir=os.path.dirname(os.path.realpath(__file__))
sys.path.append(basedir + '/./lib/Adafruit_Libraries/Adafruit_ADS1x15')

from Adafruit_ADS1x15 import ADS1x15

ADS1015 = 0x00 #12 bit ADC


class VoltageCollector(object):
    def __init__(self, argv):
        parser = OptionParser()
        parser.add_option(
            "-c",
            "--config",
            dest="config",
            default="/etc/default/voltage_daemon.cfg",
            help="Config file"
        )

        (options, args) = parser.parse_args(argv) 
        self.config = options.config
        self.adc = ADS1x15(ic=ADS1015)

        with open(self.config, 'r') as f:
            self.configuration = json.loads(f.read())

    def translate_volts(self, sensor):
        """ for 
        vin_true = 3.298
        vout_true = .779
        """

        index, data = sensor
        vout = float(data)
        z1 = int(self.configuration[str(index)]['coefficient']['z1'])
        z2 = int(self.configuration[str(index)]['coefficient']['z2'])
        vin = vout * (z1+z2) / z2
        return vin

    def loop_once(self):
        sensors = self.configuration.keys()

        volts = map(lambda x: self.adc.readADCSingleEnded(x, 4096, 250) / 1000, sensors)
        writer = partial(dataWriter, prefix='/var/tmp/data')
        map(lambda (name, volt): writer(name, volt), zip(['voltage.raw.a{n}'.format(n=x) for x in sensors], volts))
        map(lambda (name, volt): writer(name, volt), zip(['voltage.multiplied.a{n}'.format(n=x) for x in sensors], map(self.translate_volts, zip(sensors,volts))))

class VoltageDaemon(Daemon):
    def run(self):
        self.voltage_collector = VoltageCollector(sys.argv[1:])
        while True:
            self.voltage_collector.loop_once()
            time.sleep(1)


def main(argv):
    myDaemon = VoltageDaemon('/tmp/voltage.pid')
    daemon_args = argv
    if '--' in argv:
        index = argv.index('--')
        app_args = [argv[0]] + argv[index+1:]
        daemon_args = argv[:index]
        sys.argv = app_args


    if 'start' == daemon_args[1]:
        myDaemon.start()
    elif 'stop' == daemon_args[1]:
        myDaemon.stop()
    elif 'restart' == daemon_args[1]:
        myDaemon.restart()
    elif 'run' == daemon_args[1]:
        myDaemon.run()
    else:
        print 'usage: {script} start|stop|restart|run'.format(script=daemon_args[0])
        sys.exit(2)
    sys.exit(0)

if __name__ == "__main__":
    main(sys.argv)

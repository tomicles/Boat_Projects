#!/usr/bin/python

import sys
import time

import gps

from functools import partial

from daemon import Daemon
from util import dataWriter

class GpsDaemon(Daemon):
    def run(self):
        session = gps.gps('localhost', '2947')
        session.stream(gps.WATCH_ENABLE | gps.WATCH_NEWSTYLE)
        writer = partial(dataWriter, prefix='/var/tmp/data')
        while True:
            report = session.next()

            if report['class'] == 'TPV':
                if hasattr(report, 'speed'):
                    writer('gps.speed', report.speed)
            time.sleep(1)


def main(argv):
    myDaemon = GpsDaemon('/tmp/gps.pid')
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
        print 'usage: {script} start|stop|restart|run [-- app_args*]'.format(script=daemon_args[0])
        sys.exit(2)
    sys.exit(0)

if __name__ == "__main__":
    main(sys.argv)

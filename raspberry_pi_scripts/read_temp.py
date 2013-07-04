#!/usr/bin/python

import sys
import time

from daemon import Daemon
from temp_collector import TempCollector


class TempDaemon(Daemon):
    def run(self):
        temp_collector = TempCollector(sys.argv[1:])
        while True:
            temp_collector.loop_once()
            time.sleep(1)


def main(argv):
    myDaemon = TempDaemon('/tmp/temp.pid')
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

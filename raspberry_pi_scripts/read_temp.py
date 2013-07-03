import sys
import time

from daemon import Daemon
from temp_collector import TempCollector


class TempDaemon(Daemon):
    def run(self):
        temp_collector = TempCollector(['sys','cable','water'])
        while True:
            temp_collector.loop_once()
            time.sleep(1)


def main(argv):
    myDaemon = TempDaemon('/tmp/temp.pid')

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

import json
from functools import partial
from optparse import OptionParser

from temp import Temp
from util import dataWriter

class TempCollector(object):
    
    def __init__(self, argv, fopen=open):
        parser = OptionParser()
        parser.add_option(
            "-c",
            "--config",
            dest="config",
            default="/etc/default/temp_daemon.cfg",
            help="Config file"
        )

        (options, args) = parser.parse_args(argv) 
        self.config = options.config

        with open(self.config, 'r') as f:
            self.configuration = json.loads(f.read())

        self.temp_indexes = self.configuration.keys()#['sys', 'cable', 'water']
        self.temp_names = ['temp.{index}'.format(index=x) for x in self.temp_indexes]

        def make_temp(index):
            return Temp(index, self.get_file(index))

        self.temps = map(lambda x: make_temp(x), self.temp_indexes)

        self.writer = partial(dataWriter, prefix='/var/tmp/data')

    def get_file(self, index):
        return self.configuration[index]['file']


    def loop_once(self):
        map(lambda (name,temp): self.writer(name, temp.read_temp()), zip(self.temp_names, self.temps))

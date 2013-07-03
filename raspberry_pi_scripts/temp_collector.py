from functools import partial
from temp import Temp
from util import dataWriter

class TempCollector(object):
    def __init__(self, indexes, fopen=open):
        self.temp_indexes = indexes#['sys', 'cable', 'water']
        self.temp_names = ['temp.{index}'.format(index=x) for x in indexes]
        self.temps = map(lambda x: Temp(x), self.temp_names)

        self.writer = partial(dataWriter, prefix='/var/tmp/data')


    def loop_once(self):
        map(lambda (name,temp): self.writer(name, temp.read_temp()), zip(self.temp_names, self.temps))

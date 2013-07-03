from functools import partial
from temp import Temp

class TempCollector(object):
    def __init__(self, names, fopen=open):
        self.temp_names = names#['sys', 'cable', 'water']
        self.temps = map(lambda x: Temp(x), self.temp_names)
        self.open = fopen #for testing ........ nope
        def dataWriter(file, data, prefix=''):
            with self.open('{prefix}{file}'.format(prefix=prefix, file=file), 'w') as f:
                f.write(data)

        self.writer = partial(dataWriter, prefix='/var/tmp/data/')


    def loop_once(self):
        map(lambda (name,temp): self.writer(name, temp.read_temp()), zip(self.temp_names, self.temps))

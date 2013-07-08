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
        #This operation on the right is equivalent to
        #self.temp_names = []
        #for index in self.temp_indexes:
        #    self.temp_names.push('temp.{index}'.format(index=index))
        #
        #It creates a new list of counter_names from the list of strings in self.temp_indexes 
        self.temp_names = ['temp.{index}'.format(index=x) for x in self.temp_indexes]

        def make_temp(index):
            return Temp(index, self.get_file(index))

        #This operation maps (applies) the fuinction make_temp() to each element in temp_indexes
        #returning a new list of Temp objects.
        #eq to
        #self.temps = []
        #for i in self.temp_indexes:
        #    self.temps.push(make_temp(i))
        self.temps = map(lambda x: make_temp(x), self.temp_indexes)

        #partial comes from functools and basically creates a new function out of an old one,
        #except part of that function is fulfilled.
        #for example here: dataWriter(counter, value, prefix) is the initial function
        #we are fulfilling prefix, generating a new function
        #writer(counter, value), where the value of prefix is fillled in as /var/tmp/data
        self.writer = partial(dataWriter, prefix='/var/tmp/data')

    def get_file(self, index):
        return self.configuration[index]['file']


    def loop_once(self):
        #This operation zips together 2 lists [a,a] and [b,b,b,b,b,b] to form a new list [(a,b),(a,b)]
        #It then does as above and maps a function over each element in that resulting list.
        #THis time each element of the list has 2 values, name and temp.
        #the function to be applied is writer.
        #can be written as
        #for (name,temp) in zip(self.temp_names, self.temps):
        #    self.writer(name, temp.read_temp())
        # 
        #The code above is subtly different, but it doesnt matter in this case.
        #Technically map returns a list of the results of the operations, but we are not recording or returning that list.
        map(lambda (name,temp): self.writer(name, temp.read_temp()), zip(self.temp_names, self.temps))

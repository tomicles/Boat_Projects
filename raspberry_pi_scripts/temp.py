import os
import time


class Temp(object):
    """
    An object that will encapsulate reading data from one of the system files associated with a temperature sensor
    """

    def __init__(self, type, file):
        self.type = type
        self.file = file

    def __read_temp_raw2(self):
        with open(self.file, 'r') as f:
            lines = f.readlines()
            return lines

    def read_temp(self):
        lines = self.__read_temp_raw2()
        while lines[0].strip()[-3:] != 'YES':
            time.sleep(0.2)
            lines = self.__read_temp_raw2()
        equals_pos = lines[1].find('t=')
        if equals_pos != -1:
            temp_string = lines[1][equals_pos+2:]
            temp_c = float(temp_string) / 1000.0
            temp_f = temp_c * 9.0 / 5.0 + 32.0
            return temp_f

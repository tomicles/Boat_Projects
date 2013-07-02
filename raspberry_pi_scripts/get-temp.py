#!/usr/bin/python

import os
import glob
import time

os.system('modprobe w1-gpio')
os.system('modprobe w1-therm')

#cable_temp_file = '/sys/bus/w1/devices/28-0000042b8948/w1_slave'
sys_temp_file = '/sys/bus/w1/devices/28-0000047accc8/w1_slave'
#water_temp_file = '/sys/bus/w1/devices/28-000003cb5f1f/w1_slave'

def read_temp_raw2(file):
    with open(file, 'r') as f:
        lines = f.readlines()
        return lines

def read_temp_raw(file):
    f = open(file, 'r')
    lines = f.readlines()
    f.close()
    return lines


def read_temp(file):
    lines = read_temp_raw2(file)
    while lines[0].strip()[-3:] != 'YES':
        time.sleep(0.2)
        lines = read_temp_raw(file)
    equals_pos = lines[1].find('t=')
    if equals_pos != -1:
        temp_string = lines[1][equals_pos+2:]
        temp_c = float(temp_string) / 1000.0
        temp_f = temp_c * 9.0 / 5.0 + 32.0
        return temp_f

#print "Cable Temp: %s" % (read_temp(cable_temp_file))
print "temp.sys %s" % (read_temp(sys_temp_file))
#print "Water Temp: %s" % (read_temp(water_temp_file))

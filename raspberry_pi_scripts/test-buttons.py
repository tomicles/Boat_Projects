#!/usr/bin/python

import serial
import time

ser = serial.Serial('/dev/ttyACM0', 9600)

def setFont(x):
    for i in xrange(x):
        ser.write('\x02') 
        time.sleep(.1)

def resetScreen():
    ser.write('\x03') # set font back to normal
    ser.write('\x01') # Home
    ser.write('\x0C') # clear screen


 
############################################################
#           tempScreen1()
#       Show raw temperatures
#       | Temperatures    |
#       | Water  : 12.45  |  
#       | Outside: 10.83  |  
#       | Inside : 7.28   |  
############################################################

def tempScreen1():
    ser.write('\x01') # Home
    ser.write('\x03')
    ser.write('Temperatures')
    ser.write('\x0D') # Carriage Return
    ser.write('Water  : ')
    with open ("/var/tmp/data/temp.water2.out", "r") as myfile:
        watertemp = myfile.read()
    ser.write(watertemp[:5])
    ser.write('\x0D') # Carriage Return
    ser.write('System: ')
    with open ("/var/tmp/data/temp.sys2.out", "r") as myfile:
        sys2temp = myfile.read()
    ser.write(sys2temp[:5])
    ser.write('\x0D') # Carriage Return
    ser.write('Inside : ')
    with open ("/var/tmp/data/temp.inside2.out", "r") as myfile:
        inside2temp = myfile.read()
    ser.write(inside2temp[:5])








resetScreen()

while True:
    buttonvalue = ser.readline()

    print buttonvalue

    tempScreen1()





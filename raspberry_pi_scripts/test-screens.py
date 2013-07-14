#!/usr/bin/python

import serial
import time

ser = serial.Serial('/dev/ttyACM0', 9600, timeout=0)

def setFont(x):
    for i in xrange(x):
        ser.write('\x02') 
        time.sleep(.1)

def resetScreen():
    ser.write('\x03') # set font back to normal
    ser.write('\x01') # Home
    ser.write('\x0C') # clear screen

def titleScreen(title):
    ser.write('\x01') # Home
    ser.write('\x03') # set font back to normal    
    setFont(1)
    ser.write(title)
    time.sleep(2)
    ser.write('\x01') # Home
    ser.write('\x03') # set font back to normal
    ser.write('\x0C') # clear screen     


 
############################################################
#       tempScreen1()
#       Show raw temperatures
#       | Temperatures    |
#
#       | System : 12.42  |
#       | Water  : 12.45  |  
#       | Outside: 10.83  |  
#       | Inside : 7.28   |  
############################################################

def tempScreen1():
    ser.write('System : ')
    with open ("/var/tmp/data/temp.sys2.out", "r") as myfile:
        sys2temp = myfile.read()
    ser.write(sys2temp[:5])
    ser.write('\x0D') # Carriage Return    
    ser.write('Water  : ')
    with open ("/var/tmp/data/temp.water2.out", "r") as myfile:
        watertemp = myfile.read()
    ser.write(watertemp[:5])
    ser.write('\x0D') # Carriage Return
    ser.write('Outside: ')
    with open ("/var/tmp/data/temp.outside2.out", "r") as myfile:
        outside2temp = myfile.read()
    ser.write(outside2temp[:5])
    ser.write('\x0D') # Carriage Return
    ser.write('Inside : ')
    with open ("/var/tmp/data/temp.inside2.out", "r") as myfile:
        inside2temp = myfile.read()
    ser.write(inside2temp[:5])


############################################################
#       voltageScreen1()
#       Show raw voltages
#       | Raw Voltage     |
#
#       | a0     : 0.0    |
#       | a1     : 0.0    |  
#       | a2 Amps: 2.11   |  
#       | a3 Batt: 3.42   |  
############################################################

def voltageScreen1():
    ser.write('a0     : ')
    with open ("/var/tmp/data/voltage.raw.a0.out", "r") as myfile:
        a0r = myfile.read()
    ser.write(a0r[:5])
    ser.write('\x0D') # Carriage Return    
    ser.write('a1     : ')
    with open ("/var/tmp/data/voltage.raw.a1.out", "r") as myfile:
        a1r = myfile.read()
    ser.write(a1r[:5])
    ser.write('\x0D') # Carriage Return
    ser.write('a2 Amps: ')
    with open ("/var/tmp/data/voltage.raw.a2.out", "r") as myfile:
        a2r = myfile.read()
    ser.write(a2r[:5])
    ser.write('\x0D') # Carriage Return
    ser.write('a3 Batt: ')
    with open ("/var/tmp/data/voltage.raw.a3.out", "r") as myfile:
        a3r = myfile.read()
    ser.write(a3r[:5])


############################################################
#       voltageScreen2()
#       Show voltages
#       |     Voltage     |
#
#       | a0     : 0.0    |
#       | a1     : 0.0    |  
#       | a2 Amps: 2.11   |  
#       | a3 Batt: 3.42   |  
############################################################

def voltageScreen1():
    ser.write('a0     : ')
    with open ("/var/tmp/data/voltage.multiplied.a0.out", "r") as myfile:
        a0 = myfile.read()
    ser.write(a0[:5])
    ser.write('\x0D') # Carriage Return    
    ser.write('a1     : ')
    with open ("/var/tmp/data/voltage.multiplied.a1.out", "r") as myfile:
        a1 = myfile.read()
    ser.write(a1[:5])
    ser.write('\x0D') # Carriage Return
    ser.write('a2 Amps: ')
    with open ("/var/tmp/data/voltage.multiplied.a2.out", "r") as myfile:
        a2 = myfile.read()
    ser.write(a2[:5])
    ser.write('\x0D') # Carriage Return
    ser.write('a3 Batt: ')
    with open ("/var/tmp/data/voltage.multiplied.a3.out", "r") as myfile:
        a3 = myfile.read()
    ser.write(a3[:5])


resetScreen()
titleScreen("Temperatures")
tempScreen1()
time.sleep(5)

titleScreen("RawVoltage")
voltageScreen1()
time.sleep(5)

titleScreen("Voltage")
voltageScreen2()
time.sleep(5)



#!/usr/bin/python

import serial
import time

ser = serial.Serial('dev/ttyACM0', 9600)

resetScreen()

while True:
	buttonvalue = ser.readline()

	print buttonvalue

	tempScreen1()




def setFont(x)
	for i in xrange(x):
		ser.write('\x02') 
		time.sleep(.1)

def resetScreen()
    ser.write('\x03') # set font back to normal
    ser.write('\x01') # Home
    ser.write('\x0C') # clear screen


# def dimScreen(x)
# 	# offset by 10 to stop brightness jitter
# 	if (stableLight < (x - 5)) {
# 		ser.write('\x0F); // Display Driver Off
# 	}
# 	if (stableLight > (x + 5)) {
# 		ser.write('\x0E); // Display Driver On
# 	}
# }



 
############################################################
# 			tempScreen1()
# 		Show raw temperatures
# 		| Temperatures    |
# 		| Water  : 12.45  |  
# 		| Outside: 10.83  |  
# 		| Inside : 7.28   |  
############################################################

def tempScreen1()
	ser.write('\x01') # Home
	ser.write('\x03')
	ser.write('Temperatures")
	ser.write('\x0D') # Carriage Return
	ser.write('Water  : ")
	with open ("/var/tmp/data/temp.water.out", "r") as myfile:
    	data=myfile.read()
    	ser.write(data)
	ser.write(OneWireW_array[3])
	ser.write('\x0D') # Carriage Return
	ser.write('Outside: ")
	ser.write(OneWire2_array[3])
	ser.write('\x0D') # Carriage Return
	ser.write('Inside : ")
	ser.write(OneWire3_array[3])

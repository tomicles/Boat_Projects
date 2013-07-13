#!/usr/bin/python

import serial

ser = serial.Serial('dev/ttyACM0', 9600)

while True:
	buttonvalue = ser.readline()

	print buttonvalue

	ser.write(buttonvalue) 

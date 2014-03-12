#!/usr/bin/python

def valid_sensor_data(s):
    try:
        if 0 < float(s) < 1100 and 0 < len(s) < 7: 
        	return True
        else:
        	return False
    except ValueError:
        return False

string = ['1', '12', '123', '1234', '12345', '1.2', '1.24', '1.252', '1.2535', '12.1', '12.24', '13.203', '14.1034', '105.1', '205.25', '301.203', '402.2023', '2012.1', '1205.20', '2063.250', '2035.3025', '1.1.1', '22.2.2', '22.14.14', '205.125.3', '125.25.125.1']


for i in string:
	if valid_sensor_data(i):
		print i
	else:
		print i + "is bad data"

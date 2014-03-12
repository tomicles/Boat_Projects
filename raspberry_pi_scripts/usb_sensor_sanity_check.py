#!/usr/bin/python

import read_usb_sensors as rus

string = ['1', '12', '123', '1234', '12345', '1.2', '1.24', '1.252', '1.2535', '12.1', '12.24', '13.203', '14.1034', '105.1', '205.25', '301.203', '402.2023', '2012.1', '1205.20', '2063.250', '2035.3025', '1.1.1', '22.2.2', '22.14.14', '205.125.3', '125.25.125.1', '1000000.00']


for i in string:
    (status, val) = rus.valid_sensor_data(i, (1,5000))
    if rus.valid(status):
        print i + " was " + str(val)
    else:
        print i + "is bad data " + str(val) + " for reason " + rus.reason(status)

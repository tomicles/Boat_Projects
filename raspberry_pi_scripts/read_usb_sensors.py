import serial
import threading
import time

port0 = '/dev/ttyUSB0'
port1 = '/dev/ttyUSB1'
baud = 38400

serial0 = serial.Serial(port0, baud, timeout=0)
serial1 = serial.Serial(port1, baud, timeout=0)
serial = [serial0, serial1]

# turn on LEDs, enable 300ms response
for device in serial:
    device.write("L1\r")
    device.write("C\r")

def read_from_port(ser):
    buffer = ""
    while True:
        data = ser.read()
    	if (data == "\r" and buffer != ""):
    		#print buffer
                    # identify the string ttyUSB0
                    value = str(ser)
                    value2 = value.split(',')[1].split('/')[2]
                    value2 = value2[:-1]
                    with open("/var/tmp/data/water.%s.out" % value2, "w+") as f:
                            f.write("%s" % buffer)
                    buffer = ""
                    time.sleep(10)
    		#return buffer

    	else:
    		buffer = buffer + data

#sensor = read_from_port(serial1)
#print sensor

thread0 = threading.Thread(target=read_from_port, args=(serial0,))
thread1 = threading.Thread(target=read_from_port, args=(serial1,))
thread0.start()
thread1.start()

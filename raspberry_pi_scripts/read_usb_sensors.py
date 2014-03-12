import serial
import threading
import time

port0 = '/dev/ttyUSB0'
port1 = '/dev/ttyUSB1'
baud = 38400

serial0 = serial.Serial(port0, baud, timeout=0)
serial1 = serial.Serial(port1, baud, timeout=0)
serial = [serial0, serial1]

# Sanity check for serial string data
# prevent outlier numbers and multiple decimal point issues
# 0-1100 as valid float less than 7 characters in length
def valid_sensor_data(s):
    try:
        if 0 < float(s) < 1100 and 0 < len(s) < 7: 
            return True
        else:
            return False
    except ValueError:
        return False


# turn on LEDs, enable 300ms response
for device in serial:
    device.write("L1\r")
    device.write("C\r")

def read_from_port(ser):
    buffer = ""
    while True:
        data = ser.read()
        if (data == "\r" and buffer != ""):
            value = str(ser)
            value2 = value.split(',')[1].split('/')[2]
            value2 = value2[:-1]
            if valid_sensor_data(buffer):
                with open("/var/tmp/data/water.%s.out" % value2, "w+") as f:
                        f.write("%s" % buffer)
            buffer = ""
            time.sleep(10)
        else:
            buffer = buffer + data

thread0 = threading.Thread(target=read_from_port, args=(serial0,))
thread1 = threading.Thread(target=read_from_port, args=(serial1,))
thread0.start()
thread1.start()

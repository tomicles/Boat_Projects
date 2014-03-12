import serial
import threading
import time
import decimal as d


__ranges = {
    'ph' : (1, 10),
    'orp' : (1, 1000)
}

__GOOD = 0
__BAD_RANGE = 1
__BAD_DATA = 2

__reasons = {
    __GOOD : "Good data",
    __BAD_RANGE : "Data was out of range",
    __BAD_DATA : "Data format was improper"
}

def valid(status):
    return status == __GOOD

def reason(status):
    return __reasons.get(status)

def valid_sensor_data(s, range):
    (low, high) = range
    try:
        with d.localcontext() as ctx:
            ctx.prec = 2
            ctx.rounding = d.ROUND_FLOOR
            num = float("{0:.2f}".format(d.Decimal(s)))

            if num < low or num > high:
                return (__BAD_RANGE, num)
            return (__GOOD, num)
    except d.InvalidOperation:
        return (__BAD_DATA, 0)


def read_from_port(ser, type):
    buffer = ""
    while True:
        data = ser.read()
        if (data == "\r" and buffer != ""):
            value = str(ser)
            value2 = value.split(',')[1].split('/')[2]
            value2 = value2[:-1]
            (status, val) = valid_sensor_data(buffer, __ranges.get(type))
            if valid(status):
                with open("/var/tmp/data/water.{sensor}.out".format(sensor=value2), "w+") as f:
                        f.write("{value}".format(value=val))
            buffer = ""
            time.sleep(10)
        else:
            buffer = buffer + data

if __name__ == "__main__":
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

    thread0 = threading.Thread(target=read_from_port, args=(serial0, "ph",))
    thread1 = threading.Thread(target=read_from_port, args=(serial1, "orp",))
    thread0.start()
    thread1.start()

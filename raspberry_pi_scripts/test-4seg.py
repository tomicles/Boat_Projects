import serial
import time
ser = serial.Serial(port='/dev/ttyUSB2')

# Clear Screen
ser.write('\x76')
time.sleep(.5)
ser.write("1245")
time.sleep(.5)

#Write Decimal
ser.write('\x77')
ser.write('\x01') #_.___
time.sleep(.5)
ser.write('\x77')
ser.write('\x02') #__.__
time.sleep(.5)
ser.write('\x77')
ser.write('\x04') #___._
time.sleep(.5)
ser.write('\x77')
ser.write('\x08') #____.

#Digit Control
#Digit 1
ser.write('\x79')
ser.write('\x00')
ser.write('12')

#Digit 2
ser.write('\x79')
ser.write('\x02')
ser.write('74')

ser.write('\x77')
ser.write('\x02') #__.__
#Adjust brightness (0-255)
ser.write('\x7a')
ser.write('1')


ser.close()

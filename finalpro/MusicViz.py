import serial
import time

arduino = serial.Serial(port='/dev/cu.usbmodem1451101', baudrate=9600, timeout=.1)


while True:
    recieved = arduino.readline()
    if recieved != '':
        print(recieved)

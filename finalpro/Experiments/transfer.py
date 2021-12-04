import numpy as np

# print(repr(np.array2string(np.arange(100, dtype=np.uint8), separator=",", threshold=9999999)))
print(np.arange(100, dtype=np.uint8).tobytes())
print(np.arange(100, dtype=np.uint8).tobytes()*5)
# print(np.array(np.arange(100, dtype=np.uint8)[63].tobytes()))
# print(np.array_str(np.arange(100, dtype=np.uint8)))
# print(len("@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abc'"))
# import serial
# import time
#
# arduino = serial.Serial(port='/dev/cu.usbmodem1451101', baudrate=9600, timeout=.1)
# # arduino = serial.Serial(port='/dev/cu.usbserial-1430', baudrate=9600, timeout=.1)
#
# img = bytes()
#
# with open('image2.jpg', 'wb+') as f:
#     f.write('hello'.encode())
#
# while True:
#     recieved = str(arduino.readline())
#     print(recieved)
#     if recieved == "b'a'":
#         for j in range(20):
#             abc = np.array2string(np.full((1,20), j))
#             arduino.write(abc.encode())
#             # sleep(0.1);
#             print("done")
#             while(str(arduino.readline())[-2] != "b"):
#                 print("waiting...")
#                 continue;
#             print("out of wait")
#         print("exit")

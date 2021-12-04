import osascript
import time

while True:
    result = osascript.osascript('get volume settings')
    print(result)
    print(type(result))
    volInfo = result[1].split(',')
    outputVol = int(volInfo[0].replace('output volume:', ''))
    print(outputVol)
    print(type(outputVol))
    print(bytes(outputVol))
    time.sleep(10)

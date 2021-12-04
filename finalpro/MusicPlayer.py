import pyaudio
import wave
import time
import serial
import glob
import osascript

#Pyaudio object which controls the actual audio playback
p = pyaudio.PyAudio()
#Serial connection to the arduino for communication
arduino = serial.Serial(port='/dev/cu.usbmodem143301', baudrate=9600, timeout=.1)

#Global variable to keep track of which song we are on
song_number = 0
#Global variable to keep track if song is paused
paused = False

#Function to read the data from the wave file
def callback(in_data, frame_count, time_info, status):
    data = wf.readframes(frame_count)
    return (data, pyaudio.paContinue)

#continuously wait for signal from arduino
while True:
    recieved = str(arduino.readline())
    print(recieved)
    #If we recieve the play music command
    if recieved == "b'playmusic'":
        time.sleep(0.01)

        # cycle through the songs each time they request a new song
        FILENAMES = glob.glob("./wav_files/*.wav")
        NUMBER_OF_SONGS = len(FILENAMES)
        song_number = (song_number + 1) % NUMBER_OF_SONGS
        #Get an open the current song
        filename = FILENAMES[song_number]
        wf = wave.open(filename, 'rb')

        # open stream using callback
        stream = p.open(format=p.get_format_from_width(wf.getsampwidth()),
                        channels=wf.getnchannels(),
                        rate=wf.getframerate(),
                        output=True,
                        stream_callback=callback)
        # start the stream
        stream.start_stream()

        while stream.is_active() or paused==True:
            #Get the computer's speakers current volume setting
            result = osascript.osascript('get volume settings')
            volInfo = result[1].split(',')
            outputVol = int(volInfo[0].replace('output volume:', ''))
            #Write the current volume to the arduino
            arduino.write(outputVol.to_bytes(2,byteorder='little'))
            #continously check for commands from the arduino
            recieved = str(arduino.readline())
            print(recieved)
            #If stop command is recieved
            if(recieved == "b'stopmusic'"):
                stream.stop_stream()
                stream.close()
                wf.close()
                break
            time.sleep(0.1)

# close PyAudio
p.terminate()

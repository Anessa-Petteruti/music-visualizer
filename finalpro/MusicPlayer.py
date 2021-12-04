import pyaudio
import wave
import time
from pynput import keyboard
import serial
import osascript
import glob


p = pyaudio.PyAudio()
arduino = serial.Serial(port='/dev/cu.usbmodem1451101', baudrate=9600, timeout=.1)

song_number = 0


paused = False    # global to track if the audio is paused

def callback(in_data, frame_count, time_info, status):
    data = wf.readframes(frame_count)
    return (data, pyaudio.paContinue)


while True:
    recieved = str(arduino.readline())
    print(recieved)

    if recieved == "b'playmusic'":
        time.sleep(0.01)
        # cycle through the songs each time they request a new song
        FILENAMES = glob.glob("./wav_files/*.wav")
        NUMBER_OF_SONGS = len(FILENAMES)
        song_number = (song_number + 1) % NUMBER_OF_SONGS

        filename = FILENAMES[song_number]
        # filename = "./wav_files/lights-30s.wav"
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
            result = osascript.osascript('get volume settings')
            volInfo = result[1].split(',')
            outputVol = int(volInfo[0].replace('output volume:', ''))
            arduino.write(outputVol.to_bytes(2,byteorder='little'))
            recieved = str(arduino.readline())
            print(recieved)
            if(recieved == "b'stopmusic'"):
                stream.stop_stream()
                stream.close()
                wf.close()
                break
            time.sleep(0.1)

        # stop stream
# close PyAudio
p.terminate()

import pyaudio
import wave
import time
from pynput import keyboard
import serial
import osascript


p = pyaudio.PyAudio()
arduino = serial.Serial(port='/dev/cu.usbmodem143301', baudrate=9600, timeout=.1)

FILENAMES = ['wav_files/CantinaBand60.wav', 'wav_files/hot-cross-buns.wav']
NUMBER_OF_SONGS = len(FILENAMES)
song_number = 0


paused = False    # global to track if the audio is paused
def on_press(key):
    global paused
    print (key)
    if key == keyboard.Key.space:
        if stream.is_stopped():     # time to play audio
            print ('play pressed')
            stream.start_stream()
            paused = False
            return False
        elif stream.is_active():   # time to pause audio
            print ('pause pressed')
            stream.stop_stream()
            paused = True
            return False
    return False

def callback(in_data, frame_count, time_info, status):
    data = wf.readframes(frame_count)
    return (data, pyaudio.paContinue)


while True:
    recieved = str(arduino.readline())
    print(recieved)

    if recieved == "b'playmusic'":
        # cycle through the songs each time they request a new song
        song_number = (song_number + 1) % NUMBER_OF_SONGS
        filename = FILENAMES[song_number]
        wf = wave.open(filename, 'rb')
        # wf = wave.open('/Users/filip/Desktop/CS/CS1600/music-visualizer/finalpro/wav_files/CantinaBand60.wav', 'rb')


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
            # print(outputVol)
            arduino.write(outputVol.to_bytes(2,byteorder='little'))
            recieved = str(arduino.readline())
            print(recieved)
            if(recieved == "b'stopmusic'"):
                # on_press(keyboard.Key.space)
                stream.stop_stream()
                stream.close()
                wf.close()
                break
            time.sleep(0.1)

        # stop stream




# you audio here


# instantiate PyAudio


# define callback






# close PyAudio
p.terminate()

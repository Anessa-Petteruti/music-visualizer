from flask import Flask
import fft

# from MusicPlayer import FILENAMES, NUMBER_OF_SONGS


# python3 -m pip install flask
# have the arduino read from your internal ipv4 address (you can find this by runing 'ipconfig' or 'ifconfig')
# python3 -m flask run --host:<ipv4address>

app = Flask(__name__)

# filename = "wav_files/CantinaBand60.wav"
song_number = 0


@app.route("/data")
def hello_world():
    # song_number += (song_number + 1) % NUMBER_OF_SONGS
    # filename = FILENAMES[song_number]
    filename = 'wav_files/hot-cross-buns.wav'
    # filename = 'wav_files/CantinaBand60.wav'
    string = fft.freq_samples(filename)
    output = len(string).to_bytes(2,byteorder='little') + string
    print("len output:::")
    print(len(output))
    # print(output)
    # print(string)
    return len(string).to_bytes(2,byteorder='little') + string

@app.route('/helloesp')
def helloHandler():
    return 'Hello ESP8266, from Flask'



if __name__ == '__main__':
    app.run(host='0.0.0.0')

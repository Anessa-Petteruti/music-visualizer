from flask import Flask
import fft


# python3 -m pip install flask
# have the arduino read from your internal ipv4 address (you can find this by runing 'ipconfig' or 'ifconfig')
# python3 -m flask run --host:<ipv4address>

app = Flask(__name__)

filename = "wav_files/CantinaBand60.wav"


@app.route("/data")
def hello_world():
    string = fft.freq_samples(filename)
    output = len(string).to_bytes(2,byteorder='little') + string
    print("len output:::")
    print(len(output))
    # print(string)
    return len(string).to_bytes(2,byteorder='little') + string

@app.route('/helloesp')
def helloHandler():
    return 'Hello ESP8266, from Flask'



if __name__ == "__main__":
  app.run(host='0.0.0.0')
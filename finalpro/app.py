from flask import Flask
import fft


# python3 -m pip install flask
# have the arduino read from your internal ipv4 address (you can find this by runing 'ipconfig' or 'ifconfig')
# python3 -m flask run --host:<ipv4address> 

app = Flask(__name__)

filename = "wav_files/CantinaBand60.wav"


@app.route("/")
def hello_world():
    string = fft.freq_samples(filename)
    return string



if __name__ == "__main__":
  app.run(port=5000)

  
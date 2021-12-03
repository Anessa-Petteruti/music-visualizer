from flask import Flask, flash, request, redirect, url_for
from werkzeug.utils import secure_filename
import os
import fft
import glob
import os
import wave
import contextlib


# python3 -m pip install flask
# have the arduino read from your internal ipv4 address (you can find this by runing 'ipconfig' or 'ifconfig')
# python3 -m flask run --host:<ipv4address>
app = Flask(__name__)


ALLOWED_EXTENSIONS = {'wav'}
app.config['UPLOAD_FOLDER'] = 'wav_files'
song_number = 0


@app.route("/data")
def hello_world():
    global song_number
    FILENAMES = glob.glob("./wav_files/*.wav")
    NUMBER_OF_SONGS = len(FILENAMES)
    song_number = (song_number + 1) % NUMBER_OF_SONGS
    print(FILENAMES)
    filename = FILENAMES[song_number]
    # filename = "./wav_files/lights-30s.wav"

    string = fft.freq_samples(filename)
    output = len(string).to_bytes(2,byteorder='little') + string
    print("len output:::")
    print(len(output))

    return len(string).to_bytes(2,byteorder='little') + string

def allowed_file(filename):
    return '.' in filename and \
        filename.rsplit('.', 1)[1].lower() in ALLOWED_EXTENSIONS



@app.route('/submit', methods=['GET', 'POST'])
def helloHandler():
    if request.method == "POST":
        if 'file' not in request.files:
            flash('No file part')
            return redirect(request.url)
        file = request.files['file']
        # If the user does not select a file, the browser submits an
        # empty file without a filename.
        if file.filename == '':
            flash('No selected file')
            return redirect(request.url)
        if file and allowed_file(file.filename):
            filename = secure_filename(file.filename)
            with wave.open(file,'r') as f:
                frames = f.getnframes()
                rate = f.getframerate()
                duration = frames / float(rate)
                print(duration)
                if duration < 180:
                    file.save(os.path.join(app.config['UPLOAD_FOLDER'], filename))

            return redirect(request.url)

    return '''
    <!doctype html>
    <head>
    <link rel="stylesheet" href="https://fonts.googleapis.com/css?family=Roboto">
    <style>
        body {
            font-family: "Arial";
            color: #ffde59;
        }
        h1 {
            font-family: "Arial";
            font-size: 100px;
            color: #ffde59;
            text-shadow: 1px 1px 2px black, 0 0 50px #ffde59, 0 0 5px #ffde59;
        }
        h2 {
            font-family: "Arial";
            font-size: 60px;
            color: #ffde59;
        }
        input {
            border-radius: 3px;
            border-color: #ffde59;
            border-width: 3px;
            background-color: #000000;
            color: #ffde59;
            font-family: "Arial";
            font-size: 30px;
        }
        label {
            font-family: "Arial";
            border-radius: 3px;
            border-color: #ffde59;
            border-width: 3px;
            background-color: #000000;
            color: #ffde59;
            cursor: pointer;
            font-size: 20px;
            margin-bottom: 40px;
            margin-right: 10px;
        }
        input[type="file"] {
            display: none;
        }
        form {
            display: flex;
            flex-direction: column;
        }
    </style>
    </head>
    <title>Arduino Song Upload</title>
    <body style="display: flex; flex-direction: column; align-items: center; justify-content:center;
    color: white; background-color: #000000">
        <h1>MUSIC VISUALIZER</h1>
        <h2>Upload Your .wav File</h2>
            <form method=post enctype=multipart/form-data>
                <label>
                    <input type=file name=file>
                    Choose File
                </label>
                <input type=submit value=Upload>
            </form>
    </body>
    '''



if __name__ == '__main__':
    app.run(host='0.0.0.0', port='4001')

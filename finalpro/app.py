from flask import Flask, flash, request, redirect, url_for
from werkzeug.utils import secure_filename
import os
import fft


# python3 -m pip install flask
# have the arduino read from your internal ipv4 address (you can find this by runing 'ipconfig' or 'ifconfig')
# python3 -m flask run --host:<ipv4address>
app = Flask(__name__)

# filename = "wav_files/CantinaBand60.wav"
FILENAMES = ['wav_files/CantinaBand60.wav', 'wav_files/hot-cross-buns.wav']
ALLOWED_EXTENSIONS = {'wav'}
app.config['UPLOAD_FOLDER'] = 'wav_files'
NUMBER_OF_SONGS = len(FILENAMES)
song_number = 0


@app.route("/data")
def hello_world():
    global song_number
    global NUMBER_OF_SONGS
    global FILENAMES
    song_number = (song_number + 1) % NUMBER_OF_SONGS
    filename = FILENAMES[song_number]
    # filename = 'wav_files/hot-cross-buns.wav'
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
            file.save(os.path.join(app.config['UPLOAD_FOLDER'], filename))
            return redirect(request.url)

    return '''
    <!doctype html>
    <title>Arduino Song Upload</title>
    <h1>Upload new .wav file to play</h1>
    <form method=post enctype=multipart/form-data>
      <input type=file name=file>
      <input type=submit value=Upload>
    </form>
    '''



if __name__ == '__main__':
    app.run(host='0.0.0.0')

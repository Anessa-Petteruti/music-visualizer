from flask import Flask, flash, request, redirect, url_for
from werkzeug.utils import secure_filename
import os
import fft
import glob

# python3 -m pip install flask
# have the arduino read from your internal ipv4 address (you can find this by runing 'ipconfig' or 'ifconfig')
# Run: flask run -h 0.0.0.0
app = Flask(__name__)

ALLOWED_EXTENSIONS = {'wav'}
app.config['UPLOAD_FOLDER'] = 'wav_files'

song_number = 0

#Route which the arduino queries to retrieve the frequency data for the current song
@app.route("/data")
def hello_world():
    global song_number
    #Find all of the valid songs in rotation
    FILENAMES = glob.glob("./wav_files/*.wav")
    NUMBER_OF_SONGS = len(FILENAMES)
    song_number = (song_number + 1) % NUMBER_OF_SONGS
    filename = FILENAMES[song_number]
    #Get the list of LED ids that need to be lit up for this song
    string = fft.freq_samples(filename)
    #Attatch information about the song length for the arduino to know when the song ends
    output = len(string).to_bytes(2,byteorder='little') + string
    print("len output:::")
    print(len(output))

    return output


#Function to ensure that people upload only allowed files
def allowed_file(filename):
    return '.' in filename and \
        filename.rsplit('.', 1)[1].lower() in ALLOWED_EXTENSIONS

#Route which allows users to upload their own songs to the visualizer
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
            #If the file is valid, save it to the folder so it enters the rotation
            filename = secure_filename(file.filename)
            file.save(os.path.join(app.config['UPLOAD_FOLDER'], filename))
            return redirect(request.url)

    return '''
    <!doctype html>
    <title>Arduino Song Upload</title>
    <body style="display: flex; flex-direction: column; align-items: center; justify-content:center;
    color: white; background-color: #323233">
        <h1>Upload new .wav file to play</h1>
            <form method=post enctype=multipart/form-data>
                <input type=file name=file>
                <input type=submit value=Upload>
            </form>
    </body>
    '''



if __name__ == '__main__':
    app.run(host='0.0.0.0')

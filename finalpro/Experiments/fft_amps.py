from __future__ import print_function
import scipy.io.wavfile as wavfile
import scipy
import scipy.fftpack
from scipy.fftpack import fft
from scipy.signal import argrelextrema
import numpy as np
from matplotlib import pyplot as plt

filename = "wav_files/CantinaBand60.wav"

fs_rate, data = wavfile.read(filename)
fftres = fft(data)

# for i in range(1, len(fftres)):
#     print(i, ",", fftres[i], "\n")

fs_rate = 44100 # Or whatever the actual sample rate is (Hz)
f = np.linspace(fs_rate,len(data), endpoint=False)
plt.plot(f, abs (fftres))
title ('Magnitude spectrum of the signal')
xlabel ('Frequency (Hz)')
from __future__ import print_function
import scipy.io.wavfile as wavfile
import scipy
from scipy.fftpack import fft, fftfreq
import numpy as np

# # ==============================================
time_period = 0.1 # FFT time period (in seconds). Can comfortably process time frames from 0.05 seconds - 10 seconds
# # ==============================================

#Function which takes in a wav filename and returns a list of the LED ids to light up
def freq_samples(filename):
    fs_rate, signal_original = wavfile.read(filename)
    total_time = int(np.floor(len(signal_original)/fs_rate))
    sample_range = np.arange(0,total_time,time_period)
    total_samples = len(sample_range)

    print ("Frequency sampling", fs_rate)
    print ("total time: ", total_time)
    print ("sample time period: ", time_period)
    print ("total samples: ", total_samples)
    print("sample range: ", sample_range)

    output_array = []
    output_tuple_amp_freq = []
    freq_ind_list = []
    final_fa = []

    freq_output = []

    #For each "chunk" in this song
    for i in sample_range:
        print("i: ", i)

        sample_start = int(i*fs_rate)
        sample_end = int((i+time_period)*fs_rate)
        print("signal shape")
        print(signal_original.shape)
        print(len(signal_original.shape))

        #Logic in case there are multiple channels in the file
        if(len(signal_original.shape) == 2):
            signal = signal_original[sample_start:sample_end, :]
        else:
            signal = signal_original[sample_start:sample_end]

        l_audio = len(signal.shape)
        if l_audio == 2:
            signal = signal.sum(axis=1) / 2

        N = signal.shape[0] # number of samplings
        #print ("Complete Samplings N", N)

        secs = N / float(fs_rate) # number of samplings divided by sampling rate = total seconds
        # print ("secs", secs)
        Ts = 1.0/fs_rate # sampling interval in time
        t = scipy.arange(0, secs, Ts) # time vector as scipy arange field / numpy.ndarray

        #This fft function gets the amplitudes essentially
        FFT = abs(fft(signal)) # 2-d array where dimensions are: sampling intervals x frequencies present in song during each interval
        FFT_side = FFT[range(int(N/2))] # one side FFT range

        #This fft function gets the corresponding frequencies
        freqs = scipy.fftpack.fftfreq(signal.size, t[1]-t[0])
        fft_freqs = np.array(freqs)
        freqs_side = freqs[range(int(N/2))] # one side frequency range
        fft_freqs_side = np.array(freqs_side)

        # Reduce to 0-5000 Hz
        bucket_size = 5
        buckets = 16

        FFT_side = FFT_side[0:bucket_size*buckets]
        fft_freqs_side = fft_freqs_side[0:bucket_size*buckets]


        # Combine frequencies into buckets
        FFT_side = np.array([int(sum(FFT_side[current: current+bucket_size])) for current in range(0, len(FFT_side), bucket_size)])
        fft_freqs_side = np.array([int(sum(fft_freqs_side[current: current+bucket_size])) for current in range(0, len(fft_freqs_side), bucket_size)])
        # FFT_side: Normalize (0-1)
        max_value = max(FFT_side)
        if (max_value != 0):
            FFT_side_norm = FFT_side / max_value
        else:
            FFT_side_norm = FFT_side

        # Get indices of top 5 frequencies in each of the 600 buckets:
        freq_ind = np.argpartition(-FFT_side_norm, 5)[:5]

        #Collect the frequencies for this chunk
        big_freqs = fft_freqs_side.take(freq_ind)
        #Map the frequencies to LEDs on our LED strip
        big_freqs = (big_freqs//(max(big_freqs)/60))
        freq_output.extend(big_freqs)

    #Turn list of LED ids to byte list for more efficient transfer
    freq_output = np.asarray(freq_output, dtype=np.uint8).tobytes()

    return freq_output

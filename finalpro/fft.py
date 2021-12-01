

from __future__ import print_function
import scipy.io.wavfile as wavfile
import scipy
import scipy.fftpack
from scipy.fftpack import fft
from scipy.signal import argrelextrema
import numpy as np
from matplotlib import pyplot as plt
# # ==============================================
time_period = 0.1 # FFT time period (in seconds). Can comfortably process time frames from 0.05 seconds - 10 seconds
# # ==============================================
#
# def freq_samples(filepath):
#
#
#
#     fs_rate, signal_original = wavfile.read(filepath)
#     total_time = int(np.floor(len(signal_original)/fs_rate))
#     sample_range = np.arange(0,total_time,time_period)
#     total_samples = len(sample_range)
#     print ("Frequency sampling", fs_rate)
#     print ("total time: ", total_time)
#     print ("sample time period: ", time_period)
#     print ("total samples: ", total_samples)
#     output_array = []
#
#     for i in sample_range:
#         # print ("Processing: %d / %d (%d%%)" % (i/time_period + 1, total_samples, (i/time_period + 1)*100/total_samples))
#         sample_start = int(i*fs_rate)
#         sample_end = int((i+time_period)*fs_rate)
#         signal = signal_original[sample_start:sample_end]
#         l_audio = len(signal.shape)
#         #print ("Channels", l_audio)
#         if l_audio == 2:
#             signal = signal.sum(axis=1) / 2
#         N = signal.shape[0]
#         #print ("Complete Samplings N", N)
#         secs = N / float(fs_rate)
#         # print ("secs", secs)
#         Ts = 1.0/fs_rate # sampling interval in time
#         #print ("Timestep between samples Ts", Ts)
#         t = scipy.arange(0, secs, Ts) # time vector as scipy arange field / numpy.ndarray
#         FFT = abs(fft(signal))
#         FFT_side = FFT[range(int(N/2))] # one side FFT range
#         freqs = scipy.fftpack.fftfreq(signal.size, t[1]-t[0])
#         fft_freqs = np.array(freqs)
#         freqs_side = freqs[range(int(N/2))] # one side frequency range
#         fft_freqs_side = np.array(freqs_side)
#         # Reduce to 0-5000 Hz
#         bucket_size = 5
#         buckets = 16
#         FFT_side = FFT_side[0:bucket_size*buckets]
#         fft_freqs_side = fft_freqs_side[0:bucket_size*buckets]
#         # Combine frequencies into buckets
#         FFT_side = np.array([int(sum(FFT_side[current: current+bucket_size])) for current in range(0, len(FFT_side), bucket_size)])
#         fft_freqs_side = np.array([int(sum(fft_freqs_side[current: current+bucket_size])) for current in range(0, len(fft_freqs_side), bucket_size)])
#         # FFT_side: Normalize (0-1)
#         max_value = max(FFT_side)
#         if (max_value != 0):
#             FFT_side_norm = FFT_side / max_value
#         # Append to output array
#         output_array.append(FFT_side_norm)
#     # Numpy array size=600x16 that has 600 rows of sampling freqs, with 16 Hz values per row
#     output_array_np = np.asarray(output_array)
#     # print(output_array_np)
#
#     # return np.array2string(output_array_np.flatten(), separator=", ", threshold=9999999 )
#     return (np.arange(60, dtype=np.uint8)%60).tobytes()

    # ============================================


def freq_samples(filename):
    fs_rate, signal_original = wavfile.read(filename)
    total_time = int(np.floor(len(signal_original)/fs_rate))
    sample_range = np.arange(0,total_time,time_period)
    total_samples = len(sample_range)

    print ("Frequency sampling", fs_rate)
    print ("total time: ", total_time)
    print ("sample time period: ", time_period)
    print ("total samples: ", total_samples)

    output_array = []
    output_tuple_amp_freq = []
    freq_ind_list = []
    final_fa = []

    for i in sample_range:

        # print ("Processing: %d / %d (%d%%)" % (i/time_period + 1, total_samples, (i/time_period + 1)*100/total_samples))

        sample_start = int(i*fs_rate)
        sample_end = int((i+time_period)*fs_rate)
        signal = signal_original[sample_start:sample_end]

        l_audio = len(signal.shape)
        #print ("Channels", l_audio)

        if l_audio == 2:
            signal = signal.sum(axis=1) / 2
        N = signal.shape[0]
        #print ("Complete Samplings N", N)

        secs = N / float(fs_rate)
        # print ("secs", secs)
        Ts = 1.0/fs_rate # sampling interval in time
        #print ("Timestep between samples Ts", Ts)

        t = scipy.arange(0, secs, Ts) # time vector as scipy arange field / numpy.ndarray

        FFT = abs(fft(signal))
        FFT_side = FFT[range(int(N/2))] # one side FFT range

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

        # Get indices of top 5 frequencies in each of the 600 buckets:
        freq_ind = np.argpartition(-FFT_side_norm, 5)[:5]
        # print(freq_ind)

        # Calculate amplitudes of FFT frequency values:
        amplitudes = 1/total_samples * np.abs(FFT_side_norm)
        # print(amplitudes.shape)

        # Add amps and freqs to dictionary:
        output_tuple_amp_freq.append((amplitudes, FFT_side_norm))


        for tupley in range(len(output_tuple_amp_freq)):
            # Get corresponding amplitudes at these indices:
            top_freqs = [output_tuple_amp_freq[tupley][1][i] for i in freq_ind]

        big_freqs = (fft_freqs_side.take(freq_ind)//(5000/60))


        final_fa.append((amplitudes, top_freqs))

    # dct = dict(map(reversed, final_fa))
    # print(dct)

    final_fa_np = np.asarray(final_fa)
    print(final_fa_np.shape)
    print(freq_ind)
    print(fft_freqs_side.take(freq_ind))
    print(big_freqs)


    # print(final_fa[0][0].shape)
    # print(final_fa[0])
    # print(final_fa[1])
    # for i in range(40):
    #     print(final_fa[i])

    #TODO: fix this!!
    return (np.arange(60, dtype=np.uint8)%60).tobytes()

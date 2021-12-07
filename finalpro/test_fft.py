import numpy as np
from fft import freq_samples

SINGLE_WAVE_TEST_FILE = 'wav_files/1000_hz_single.wav'
RAW_SINGLE_WAVE = [
    51, 60, 29, 42, 56, 51, 60, 29, 42, 56,
    51, 60, 29, 42, 56, 51, 60, 29, 42, 56,
    51, 60, 29, 42, 56, 51, 60, 29, 42, 56,
    51, 60, 29, 42, 56, 51, 60, 29, 42, 56,
    51, 60, 29, 42, 56, 51, 60, 29, 42, 56,
    51, 60, 29, 42, 56, 51, 60, 29, 42, 56,
    51, 60, 29, 42, 56, 51, 60, 29, 42, 56,
    51, 60, 29, 42, 56, 51, 60, 29, 42, 56,
    51, 60, 29, 42, 56, 51, 60, 29, 42, 56,
    51, 60, 29, 42, 56, 51, 60, 29, 42, 56,
    51, 60, 29, 42, 56, 51, 60, 29, 42, 56,
    51, 60, 29, 42, 56, 51, 60, 29, 42, 56,
    51, 60, 29, 42, 56, 51, 60, 29, 42, 56,
    51, 60, 29, 42, 56, 51, 60, 29, 42, 56,
    51, 60, 29, 42, 56, 60, 52, 44, 36, 48
]
EXPECTED_SINGLE_WAVE = np.asarray(RAW_SINGLE_WAVE, dtype=np.uint8).tobytes()

FOUR_TONES_TEST_FILE = 'wav_files/four_tones_test.wav'
RAW_FOUR_TONES = [
    51, 60, 10, 47, 39, 51, 60, 10, 47, 39,
    51, 60, 10, 47, 39, 51, 60, 10, 47, 39,
    51, 60, 10, 47, 39, 51, 60, 10, 47, 39,
    51, 60, 10, 47, 39, 20, 60, 51, 56, 42,
    20, 60, 51, 56, 42, 51, 60, 10, 47, 39,
    51, 60, 10, 47, 39, 20, 60, 51, 56, 42,
    51, 60, 10, 47, 39, 51, 60, 10, 47, 39,
    51, 60, 10, 47, 39, 51, 60, 10, 47, 39,
    51, 60, 10, 47, 39, 51, 60, 10, 47, 39,
    51, 60, 10, 47, 39, 51, 60, 10, 47, 39,
    51, 60, 10, 47, 39, 51, 60, 10, 47, 39,
    51, 60, 10, 47, 39, 51, 60, 10, 47, 39,
    51, 60, 10, 47, 39, 51, 60, 10, 47, 39,
    51, 60, 10, 47, 39, 51, 60, 10, 47, 39,
    51, 60, 10, 47, 39, 51, 60, 10, 47, 39,
    51, 60, 10, 47, 39, 51, 60, 10, 47, 39,
    51, 60, 10, 47, 39, 51, 60, 10, 47, 39,
    51, 60, 10, 47, 39, 51, 60, 10, 47, 39,
    55, 6, 50, 46, 60, 55, 6, 50, 46, 60, 55,
    6, 50, 46, 60, 51, 60, 10, 47, 39, 51, 60,
    10, 47, 39, 51, 60, 10, 47, 39, 51, 60, 10,
    47, 39, 51, 60, 10, 47, 39, 51, 60, 10, 47,
    39, 51, 60, 10, 47, 39, 51, 60, 10, 47, 39,
    51, 60, 10, 47, 39, 51, 60, 10, 47, 39, 51,
    60, 10, 47, 39, 51, 60, 10, 47, 39, 51, 60,
    10, 47, 39, 51, 60, 10, 47, 39, 51, 60, 10,
    47, 39, 51, 60, 10, 47, 39, 51, 60, 10, 47,
    39, 51, 60, 10, 47, 39, 51, 60, 10, 47, 39,
    51, 60, 10, 47, 39, 51, 60, 10, 47, 39, 51,
    60, 10, 47, 39, 51, 60, 10, 47, 39, 51, 60,
    10, 47, 39, 51, 60, 10, 47, 39, 51, 60, 10,
    47, 39, 51, 60, 10, 47, 39, 51, 60, 10, 47,
    39, 51, 60, 10, 47, 39, 51, 60, 10, 47, 39,
    55, 6, 50, 46, 60, 55, 6, 50, 46, 60, 55, 6,
    50, 46, 60, 55, 6, 50, 46, 60, 55, 6, 50,
    46, 60, 55, 6, 50, 46, 60, 55, 6, 50, 46,
    60, 55, 6, 50, 46, 60, 55, 6, 50, 46, 60,
    51, 60, 10, 47, 39, 51, 60, 10, 47, 39, 51,
    60, 10, 47, 39, 51, 60, 10, 47, 39, 51, 60,
    10, 47, 39, 51, 60, 10, 47, 39, 51, 60, 10,
    47, 39, 51, 60, 10, 47, 39, 51, 60, 10, 47,
    39, 51, 60, 10, 47, 39, 51, 60, 10, 47, 39,
    51, 60, 10, 47, 39, 51, 60, 10, 47, 39, 51,
    60, 10, 47, 39, 51, 60, 10, 47, 39, 51, 60,
    10, 47, 39, 51, 60, 10, 47, 39, 51, 60, 10,
    47, 39, 51, 60, 10, 47, 39, 51, 60, 10, 47,
    39, 51, 60, 10, 47, 39, 51, 60, 10, 47, 39,
    51, 60, 10, 47, 39, 51, 60, 10, 47, 39, 51,
    60, 10, 47, 39, 51, 60, 10, 47, 39, 51, 60,
    10, 47, 39, 55, 6, 50, 46, 60, 1, 60, 51,
    42, 56, 51, 55, 47, 39, 60, 51, 55, 47, 39,
    60, 51, 55, 47, 39, 60
]
EXPECTED_FOUR_TONES = np.asarray(RAW_FOUR_TONES, dtype=np.uint8).tobytes()

def test_fft():
    single_wave_result = freq_samples(SINGLE_WAVE_TEST_FILE)
    # expect length of 150, because samples taken every 10ms, song is 3s, and taking top 5 frequencies: 30 * 5 = 150
    assert(len(single_wave_result) == 150)
    for i, val in enumerate(single_wave_result):
        assert(val == EXPECTED_SINGLE_WAVE[i])

    four_tones_result = freq_samples(FOUR_TONES_TEST_FILE)
    # expect length of 550: samples every 10ms, song is 11s, taking top 5 frequencies, 10 * 5 * 11 = 550
    assert(len(four_tones_result) == 550)
    for i, val in enumerate(four_tones_result):
        assert(val == EXPECTED_FOUR_TONES[i])

if __name__ == '__main__':
    test_fft()
    print('All Tests Passed')
from RealtimeSTT import AudioToTextRecorder

recorder_config = {
    # "spinner": False,
    # "use_microphone": False,
    "model": "small.en",
    "language": "en",
    "silero_sensitivity": 0.4,
    "webrtc_sensitivity": 2,
    "post_speech_silence_duration": 0.7,
    # "min_length_of_recording": 0,
    "min_gap_between_recordings": 0,
    "enable_realtime_transcription": True,
    # "realtime_processing_pause": 0,
    "realtime_model_type": "small.en",
    # 'on_realtime_transcription_stabilized': text_detected,
}

if __name__ == "__main__":
    from multiprocessing import freeze_support

    freeze_support()

    # Your main code goes here
    # recorder = AudioToTextRecorder(**recorder_config)
    # ... rest of your code

    recorder = AudioToTextRecorder(**recorder_config)
    # # print("methgab")
    # recorder.start()
    # # print("started")
    # recorder.stop()
    # print(recorder.text())
    # # print("end")

    # with AudioToTextRecorder() as recorder:
    #     print(recorder.text())

    def process_text(text):
        print(text)

    while True:
        recorder.text(process_text)

# import pyaudio

# # Specify the input device index
# input_device_index = 3  # Replace with the desired input device index

# # Create a PyAudio instance
# p = pyaudio.PyAudio()

# # Get the number of channels for the specified input device
# channels = p.get_device_info_by_index(input_device_index)

# # Print the result
# print("Channels =", channels)

# # Close the PyAudio instance
# p.terminate()

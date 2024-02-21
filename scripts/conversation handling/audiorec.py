import wave
import queue
import pyaudio
import webrtcvad
import threading

# Set the sample rate and other parameters for PyAudio
sample_rate = 16000
chunk_size = 1024
format = pyaudio.paInt16
channels = 1

# Set the threshold for VAD (adjust as needed)
vad_threshold = 1500

# Create a VAD instance
vad = webrtcvad.Vad()

# Set the aggressiveness level (0 to 3)
vad.set_mode(3)

# Create a PyAudio stream
p = pyaudio.PyAudio()
stream = p.open(
    format=format,
    channels=channels,
    rate=sample_rate,
    input=True,
    frames_per_buffer=chunk_size,
)

# Create a queue to store audio data
audio_queue = queue.Queue()


# Function to write audio data to a WAV file
def write_wav(filename, frames):
    with wave.open(filename, "wb") as wf:
        wf.setnchannels(channels)
        wf.setsampwidth(p.get_sample_size(format))
        wf.setframerate(sample_rate)
        wf.writeframes(b"".join(frames))


# Function to process audio data
def process_audio():
    frames = []
    while True:
        data = audio_queue.get()
        if data is None:
            break
        frames.append(data)

    # Check if the frames contain enough audio data
    if len(frames) > 0:
        # Check if the frames contain voice activity
        if vad.is_speech(b"".join(frames), sample_rate):
            print("Voice activity detected. Writing audio clip...")
            filename = f"audio_clip_{len(frames)}.wav"
            write_wav(filename, frames)
            print(f"Audio clip '{filename}' written.")


# Start a separate thread for processing audio data
processing_thread = threading.Thread(target=process_audio)
processing_thread.start()

print("Listening for audio input (Press Ctrl+C to stop)...")
try:
    while True:
        audio_data = stream.read(chunk_size)
        audio_queue.put(audio_data)
        # Adjust the threshold based on your environment and microphone sensitivity
        if max(abs(audio_data)) > vad_threshold:
            print("Audio detected")
except KeyboardInterrupt:
    print("Stopping...")

# Stop the audio processing thread
audio_queue.put(None)
processing_thread.join()

# Stop and close the PyAudio stream
stream.stop_stream()
stream.close()
p.terminate()

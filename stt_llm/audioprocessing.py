import time, logging
from datetime import datetime
import threading, collections, queue, os, os.path
import json

# from deepgram import DeepgramClient, PrerecordedOptions
from faster_whisper import WhisperModel

# DEEPGRAM_API_KEY = "5aaf9146ad386c3fd0529692e1b5bd584d5deec1"
id_ = 0
recorded = False
text = ""
text_id = -1
# import deepspeech
import numpy as np
import pyaudio
import wave
import webrtcvad
from halo import Halo
import sounddevice
from scipy import signal

logging.basicConfig(level=20)


class speech_to_text:
    def __init__(self, model_size, device, compute_type):
        self.model = WhisperModel(model_size, device=device, compute_type=compute_type)

    def transcribe_(self, audio_path, id__):
        segments, info, id____ = self.model.transcribe(
            audio_path, beam_size=5, id_=id__
        )
        text = ""
        for segment in segments:
            text += segment.text
        return text, id____


class Audio(object):
    """Streams raw audio from microphone. Data is received in a separate thread, and stored in a buffer, to be read from."""

    FORMAT = pyaudio.paInt16
    # Network/VAD rate-space
    RATE_PROCESS = 16000
    CHANNELS = 1
    BLOCKS_PER_SECOND = 50

    def __init__(self, callback=None, device=None, input_rate=RATE_PROCESS, file=None):
        def proxy_callback(in_data, frame_count, time_info, status):
            # pylint: disable=unused-argument
            if self.chunk is not None:
                in_data = self.wf.readframes(self.chunk)
            callback(in_data)
            return (None, pyaudio.paContinue)

        if callback is None:
            callback = lambda in_data: self.buffer_queue.put(in_data)
        self.buffer_queue = queue.Queue()
        self.device = device
        self.input_rate = input_rate
        self.sample_rate = self.RATE_PROCESS
        self.block_size = int(self.RATE_PROCESS / float(self.BLOCKS_PER_SECOND))
        self.block_size_input = int(self.input_rate / float(self.BLOCKS_PER_SECOND))
        self.pa = pyaudio.PyAudio()

        kwargs = {
            "format": self.FORMAT,
            "channels": self.CHANNELS,
            "rate": self.input_rate,
            "input": True,
            "frames_per_buffer": self.block_size_input,
            "stream_callback": proxy_callback,
        }

        self.chunk = None
        # if not default device
        if self.device:
            kwargs["input_device_index"] = self.device
        elif file is not None:
            self.chunk = 320
            self.wf = wave.open(file, "rb")

        self.stream = self.pa.open(**kwargs)
        self.stream.start_stream()

    def resample(self, data, input_rate):
        """
        Microphone may not support our native processing sampling rate, so
        resample from input_rate to RATE_PROCESS here for webrtcvad and
        deepspeech

        Args:
            data (binary): Input audio stream
            input_rate (int): Input audio rate to resample from
        """
        data16 = np.fromstring(string=data, dtype=np.int16)
        resample_size = int(len(data16) / self.input_rate * self.RATE_PROCESS)
        resample = signal.resample(data16, resample_size)
        resample16 = np.array(resample, dtype=np.int16)
        return resample16.tostring()

    def read_resampled(self):
        """Return a block of audio data resampled to 16000hz, blocking if necessary."""
        return self.resample(data=self.buffer_queue.get(), input_rate=self.input_rate)

    def read(self):
        """Return a block of audio data, blocking if necessary."""
        return self.buffer_queue.get()

    def destroy(self):
        self.stream.stop_stream()
        self.stream.close()
        self.pa.terminate()

    frame_duration_ms = property(
        lambda self: 1000 * self.block_size // self.sample_rate
    )

    def write_wav(self, filename, data):
        logging.info("write wav %s", filename)
        wf = wave.open(filename, "wb")
        wf.setnchannels(self.CHANNELS)
        # wf.setsampwidth(self.pa.get_sample_size(FORMAT))
        assert self.FORMAT == pyaudio.paInt16
        wf.setsampwidth(2)
        wf.setframerate(self.sample_rate)
        wf.writeframes(data)
        wf.close()


class VADAudio(Audio):
    """Filter & segment audio with voice activity detection."""

    def __init__(self, aggressiveness=3, device=None, input_rate=None, file=None):
        super().__init__(device=device, input_rate=input_rate, file=file)
        self.vad = webrtcvad.Vad(aggressiveness)

    def frame_generator(self):
        """Generator that yields all audio frames from microphone."""
        if self.input_rate == self.RATE_PROCESS:
            while True:
                yield self.read()
        else:
            while True:
                yield self.read_resampled()

    def vad_collector(self, padding_ms=300, ratio=0.75, frames=None):
        """Generator that yields series of consecutive audio frames comprising each utterence, separated by yielding a single None.
        Determines voice activity by ratio of frames in padding_ms. Uses a buffer to include padding_ms prior to being triggered.
        Example: (frame, ..., frame, None, frame, ..., frame, None, ...)
                  |---utterence---|        |---utterence---|
        """
        if frames is None:
            frames = self.frame_generator()
        num_padding_frames = padding_ms // self.frame_duration_ms
        ring_buffer = collections.deque(maxlen=num_padding_frames)
        triggered = False

        for frame in frames:
            if len(frame) < 640:
                return

            is_speech = self.vad.is_speech(frame, self.sample_rate)

            if not triggered:
                ring_buffer.append((frame, is_speech))
                num_voiced = len([f for f, speech in ring_buffer if speech])
                if num_voiced > ratio * ring_buffer.maxlen:
                    triggered = True
                    for f, s in ring_buffer:
                        yield f
                    ring_buffer.clear()

            else:
                yield frame
                ring_buffer.append((frame, is_speech))
                num_unvoiced = len([f for f, speech in ring_buffer if not speech])
                if num_unvoiced > ratio * ring_buffer.maxlen:
                    triggered = False
                    yield None
                    ring_buffer.clear()


# def speechToText(path):
#     deepgram = DeepgramClient(DEEPGRAM_API_KEY)

#     with open(path, "rb") as buffer_data:
#         payload = {"buffer": buffer_data}

#         options = PrerecordedOptions(
#             smart_format=True,
#             model="nova-2",
#             language="en-IN",
#             extra=f"id_:{id_}",
#         )

#         print("Requesting transcript...")
#         response = deepgram.listen.prerecorded.v("1").transcribe_file(payload, options)
#         return response.to_json(indent=4)


def idIncrement(previous_time):
    global id_
    global recorded
    now_ = time.time()
    if (now_ - previous_time) >= 2.5 and recorded:
        id_ += 1
        recorded = False
        return


def text_publish(response, ___id):
    global text_id, text
    # response_dic = json.loads(response)
    id_ = ___id
    new_text = response
    if id_ == text_id:
        text += new_text
    else:
        """here publish the text"""
        text_id = id_
        text = new_text
    print(text)


def main(ARGS):
    # Start audio with VAD
    save_path = ARGS
    vad_audio = VADAudio(
        aggressiveness=0,
        device=0,
        input_rate=16000,
    )
    print("Listening (ctrl-C to exit)...")
    frames = vad_audio.vad_collector()
    previous_time = 0
    speeches = []
    # Stream from microphone to DeepSpeech using VAD
    spinner = None
    if not False:
        spinner = Halo(spinner="line")
    wav_data = bytearray()
    for frame in frames:
        if frame is not None:
            idIncrement(previous_time)
            if spinner:
                spinner.start()
            if save_path:
                wav_data.extend(frame)
        else:
            if spinner:
                spinner.stop()
            logging.debug("end utterence")
            if save_path:
                file_name = os.path.join(
                    save_path,
                    datetime.now().strftime(
                        f"id_{id_}_savewav_%Y-%m-%d_%H-%M-%S_%f.wav"
                    ),
                )
                vad_audio.write_wav(
                    file_name,
                    wav_data,
                )
                text_response, ___id = speechToText.transcribe_(file_name, id_)
                text_publish(text_response, ___id)
                previous_time = time.time()
                global recorded
                recorded = True
                wav_data = bytearray()


if __name__ == "__main__":
    speechToText = speech_to_text("small.en", "cuda", "float16")
    DEFAULT_SAMPLE_RATE = 16000
    save_path = "."
    if save_path:
        os.makedirs(save_path, exist_ok=True)
    main(save_path)

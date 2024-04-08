import sounddevice
import whisper
import time
from scipy.io.wavfile import write

# fs= 44100 #44100
# second = 10# int(input("Enter time duration in seconds: "))
# print("Recording.....n")
# record_voice = sounddevice.rec( int ( second * fs ) , samplerate = fs , channels = 1 )
# sounddevice.wait()
# write("out.wav",fs,record_voice)
# print("Finished.....\nPlease check your output file")


tic = time.time()
model = whisper.load_model("base.en")
result = model.transcribe("out.wav",device='cpu')
print(result["text"])
print(time.time()-tic )
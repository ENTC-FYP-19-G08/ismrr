import speech_recognition as sr
import time

init_rec = sr.Recognizer()
print("Let's speak!!")
with sr.Microphone() as source:
    audio_data = init_rec.record(source, duration=5)
    print("Recognizing your text.............")
    tic = time.time()
    text = init_rec.recognize_google(audio_data)
    print(text)
    print(time.time()-tic )

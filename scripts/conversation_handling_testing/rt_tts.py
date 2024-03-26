from RealtimeTTS import TextToAudioStream, SystemEngine, AzureEngine, ElevenlabsEngine

engine = SystemEngine()  # replace with your TTS engine
stream = TextToAudioStream(engine)
stream.feed("Hello world! How are you today?")
stream.play()

print(engine.get_voices())
# engine.set_voice(voice="C:\Program Files (x86)\eSpeak\espeak-data\voices\asia\tr")

# import pyttsx3

# engine = pyttsx3.init()

# voices = engine.getProperty("voices")

# for voice in voices:
#     for intonation in ["+f1", "+f2", "+f3", "+f4"]:
#         print(voice.id, intonation)
#         engine.setProperty("voice", voice.id + intonation)

#         engine.say("Hello World!")
#         engine.runAndWait()

#         input("Press enter to continue ...")

from faster_whisper import WhisperModel
import time

model_size = "small.en"

# Run on GPU with FP16
model = WhisperModel(model_size, device="cuda", compute_type="float16")

tic = time.time()
segments, info = model.transcribe("audio.m4a", beam_size=5)

print(
    "Detected language '%s' with probability %f"
    % (info.language, info.language_probability)
)

for segment in segments:
    print(segment.text)
    # print("[%.2fs -> %.2fs] %s" % (segment.start, segment.end, segment.text))

print("TIME :", time.time() - tic)

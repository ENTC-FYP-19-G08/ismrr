from deepgram import DeepgramClient, PrerecordedOptions
import time

# The API key we created in step 3
DEEPGRAM_API_KEY = "fe860ec29a2db8818735d3001133ea1ba2c3e918"

# Replace with your file path
PATH_TO_FILE = "audio.m4a"


def main():
    deepgram = DeepgramClient(DEEPGRAM_API_KEY)

    with open(PATH_TO_FILE, "rb") as buffer_data:
        payload = {"buffer": buffer_data}

        options = PrerecordedOptions(
            smart_format=True, model="nova-2", language="en-IN"
        )

        print("Requesting transcript...")
        print("Your file may take up to a couple minutes to process.")
        print(
            "While you wait, did you know that Deepgram accepts over 40 audio file formats? Even MP4s."
        )
        print(
            "To learn more about customizing your transcripts check out developers.deepgram.com"
        )

        response = deepgram.listen.prerecorded.v("1").transcribe_file(payload, options)
        print(response.to_json(indent=4))


if __name__ == "__main__":
    tic = time.time()
    main()
    print("### TIME : ", time.time() - tic)

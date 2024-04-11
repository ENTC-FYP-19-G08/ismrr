import multiprocessing
import subprocess
from datetime import datetime

# import pygame
import os


class TextToSpeech:

    def __init__(self):
        self.input_queue_1 = multiprocessing.Queue()
        self.output_queue = multiprocessing.Queue()
        self.ending_queue = multiprocessing.Queue()

    def initialize_processes(self):
        self.process_1 = multiprocessing.Process(
            target=self.make_wav, args=(self.input_queue_1, self.output_queue,)
        )
        self.process_2 = multiprocessing.Process(
            target=self.play_wav, args=(self.output_queue,self.ending_queue,)
        )

        self.process_1.start()
        self.process_2.start()

        # self.process_1.join()
        # self.process_2.join()

    def kill_processess(self):
        if self.process_1.is_alive():
            self.process_1.kill()
        if self.process_2.is_alive():
            self.process_2.kill()

    def get_ending(self):
        i = self.ending_queue.get()
        return i

    def delete_wav_file(self, file_path):
        if os.path.exists(file_path):
            if file_path.endswith(".wav"):
                os.remove(file_path)
                # print(f"{file_path} deleted successfully.")
            else:
                print(f"{file_path} is not a .wav file.")
        else:
            print(f"{file_path} does not exist.")

    def play_wav_file(self, file_path):
        command = f"""aplay {file_path}"""
        result = subprocess.run(
            command,
            shell=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
        )

    # def play_wav_file(self, file_path):
    #     pygame.init()
    #     pygame.mixer.init()

    #     try:
    #         # Load the WAV file
    #         sound = pygame.mixer.Sound(file_path)

    #         # Play the sound
    #         sound.play()
    #         pygame.time.wait(int(sound.get_length() * 1000))
    #     except pygame.error as e:
    #         print(f"Error playing audio: {e}")
    #     finally:
    #         pygame.quit()
    #         return

    def make_wav(self, input_queue, output_queue):
        while True:
            item = input_queue.get()
            if item is None:  # Signal to stop processing
                output_queue.put(None)
                break
            if item=="$$":
                output_queue.put("$$")
            else:
                name = datetime.now().strftime("savewav_%Y-%m-%d_%H-%M-%S_%f.wav")
                command = f"""/SSD/build/flite/bin/flite -voice /SSD/build/flite/voices/cmu_us_lnh.flitevox -t "{item}" ./{name}"""
                result = subprocess.run(
                    command,
                    shell=True,
                    stdout=subprocess.PIPE,
                    stderr=subprocess.PIPE,
                    text=True,
                )
                output_queue.put(name)

    def play_wav(self, input_queue,d_queue):
        while True:
            item = input_queue.get()
            if item is None:  # Signal to stop processing
                break
            # Perform certain process on item
            if item =="$$":
                d_queue.put("$$")
            else:
                self.play_wav_file(item)
                self.delete_wav_file(item)
                # print("not ending")

    def convert_text_to_speech(self, text):
        self.input_queue_1.put(text)


# if __name__ == "__main__":
#     input_queue_1 = multiprocessing.Queue()
#     output_queue = multiprocessing.Queue()
#     # input_queue_2 = multiprocessing.Queue()

#     process_1 = multiprocessing.Process(
#         target=make_wav, args=(input_queue_1, output_queue)
#     )
#     process_2 = multiprocessing.Process(target=play_wav, args=(output_queue,))

#     process_1.start()
#     process_2.start()

#     # Example usage
#     input_queue_1.put("Hello")
#     input_queue_1.put("World")
#     input_queue_1.put("Good morning")
#     input_queue_1.put(
#         "hello my friends i dont know what to do at this point of my life. it is pretty hard. but i will manage it in the end of this scene"
#     )
#     # Add None to signal end of input
#     input_queue_1.put(None)

#     # Wait for processes to finish
#     print("yyihi")
#     process_1.join()
#     print("process 1 ended")
#     process_2.join()
#     print("process 2 ended")

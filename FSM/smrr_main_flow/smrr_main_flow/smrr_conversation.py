from pickle import STRING

from datetime import datetime

import time, logging, sounddevice
import threading, collections, queue, os, os.path
import multiprocessing
from faster_whisper import WhisperModel
import numpy as np
from halo import Halo
from scipy import signal
import random
# from .speech_to_text_module import VADAudio
# from .speech_to_text_module import FasterWhisper

# from .llm import LLM
# from .text_to_speech import TextToSpeech
# from .location_classifier import LocationClassifier
from location_guide import play_audio_clip
from messages import thanking_messages
from speech_to_text_module import VADAudio
from speech_to_text_module import FasterWhisper

from llm import LLM
from text_to_speech import TextToSpeech
from location_classifier import LocationClassifier

from std_msgs.msg import String


class SMRRCoversation:
    def __init__(self, node):
        self.node = node
        # self.whisper = FasterWhisper("small.en", "cuda", "float32")
        self.llm = LLM(
            m="/SSD/exllamav2_old/my_model",
            mode="llama",
            # pt=True,
            ncf = True,
            # temp=0.5,
            # smooth=0.5,
            # topk=10,
            maxr=400,   
            sp = "You are the Smart Mobile Robot Receptionist at the Department of Electronic and Telecommunication Engineering of the University of Moratuwa. ENTC is a short form that is used to refer to the Department of Electronic and Telecommunication Engineering. You have to behave like the robot receptionist at ENTC. You have to have that identity. You are operated from the third floor of the building. Almost every question and statement is related to the department and you have to answer from the context of the department. Greet appropriately and ask how you can assist them when a conversation is begun. When ending a conversation greet the user appropriately for contacting the robot receptionist. If a user asks for directions to any location in the department, directly use the given information below related to the department. If you can not find any relevant information to respond to a user question or statement, decently say you can not help and manage the situation. You have to respond to the user questions or statements delimited by triple backticks. Here is some information about the department as the receptionist you must know. The head of the department: Dr. Ranga Rodrigo. ENTC1 hall: ENTC1 hall is on the ground floor. to go to the entc1 hall, a person can take the lift from the 3rd floor go to the ground floor, and head toward the left side once the person exits the lift. lower common: the lower common area is on the ground floor. a person can take the lift from the 3rd floor go to the ground floor, and head toward the left side once the person exits the lift. bio-medical lab: the bio-medical laboratory is on the ground floor. a person can take the lift from the 3rd floor go to the ground floor, and head toward the left side once the person exits the lift. uav lab: The UAV laboratory is on the ground floor. to go to the UAV laboratory, a person can take the lift from the 3rd floor go to the ground floor, and head toward the right side once the person exits the lift. lift: if you were asked about directions to the lift, say this \"You can find the lift on your left-hand side of this floor\". common washroom: if you were asked about directions to the washroom, say this \"You can find the washroom on your left-hand side of this floor.\" staff washroom: the staff washroom is on the first floor. To go to the staff washroom, a person can take the lift from the 3rd floor go to the first floor, and the right block of the lift is the staff washroom. computer lab: The computer laboratory is on the first floor. A person can take the lift from the 3rd floor go to the first floor, and head toward the left side once the person exits the lift. lecturers rooms: The lecturers' rooms are on both the first and second floors. a person can take the lift from the 3rd floor and go to the relevant floor and find the lecturer's room. staff room: the staff room is on the first floor. A person can take the lift from the 3rd floor go to the first floor, and head toward the right side once the person exits the lift. office: the department office is on the first floor. To go to the department office, a person can take the lift from the 3rd floor go to the first floor, and go to the right side once the person exits the lift. conference room: the conference room is on the first floor. A person can take the lift from the 3rd floor go to the first floor, and go to the right side once the person exits the lift. HOD office: the head of the department's office is on the first floor. To go to the head of the department's office, a person can take the lift from the 3rd floor go to the first floor, and go to the right side once the person exits the lift. analog lab: the analog electronic laboratory is on the second floor. To go to the analog electronic laboratory, a person can take the lift from the 3rd floor go to the second floor, and head toward the left side once the person exits the lift. upper common: the upper common area is on the second floor. To go there, a person can take the lift from the 3rd floor go to the second floor, and go to the left side once the person exits the lift. workshop: the electronic workshop is on the second floor. To get there, a person can take the lift from the 3rd floor go to the second floor, and go to the right side once the person exits the lift. soldering room: the soldering room is on the second floor. To go to the soldering room, a person can take the lift from the 3rd floor and go to the second floor, it is in front of the person as the person exits the lift. digital lab: the digital electronic laboratory is on the second floor. To get there, a person can take the lift from the 3rd floor go to the second floor, and head toward the left side once the person exits the lift. telecom lab: if you were asked about directions to the telecom lab, say this \"You can find the telecom lab on your right-hand side of this floor\".vision lab: if you were asked about directions to the vision lab, say this \"You can see the vision lab on your right-hand side\". pg lab: if you were asked about directions to the PG (postgraduate) lab, say this \"You can find the PG lab on your right-hand side of this floor\". 3.5 lecture hall: if you were asked about directions to the 3.5 lecture hall, say this \"You can see the 3.5 lecture hall there\". dr. peshala: doctor peshala's room is on the first floor. To get there, a person can take the lift from the 3rd floor and go to the first floor, then the person has to go to the left as they exit the lift. Doctor Peshala's room is in the lecture rooms section on the person's right-hand side. prof. rohan: professor rohan's room is on the first floor. To get there, a person can take the lift from the 3rd floor and go to the first floor, then the person has to go to the left as they exit the lift. Prof Rohan's room is in the lecture rooms section on the person's left-hand side. prof. dileeka: professor dileeka's room is on the first floor. To get there, a person can take the lift from the 3rd floor and go to the first floor, then the person has to go to the left as they exit the lift. professor dileeka's room is in the lecture rooms section on the person's left-hand side. prof. jayasinghe: professor jayasinghe's room is on the first floor. To get there, a person can take the lift from the 3rd floor and go to the first floor, then the person has to go to the left as they exit the lift. professor jayasinghe's room is in the lecture rooms section on the person's left-hand side. instructors room: the instructors' room is on the second floor. To get there, a person can take the lift from the 3rd floor and go to the second floor, there is a passage in front of the lift door which directs to the front direction. the person has to go through that passage.",
        )
        self.language_understanding_and_generation("test run",warmup_flag=True)
        self.trig_sub = self.node.create_subscription(
            String, "/trigger", self.call_back, 10
        )
        self.ui_sub = self.node.create_subscription(String, '/ui/guide_navigation', self.guide_navigation_callback, 10)
        self.ui_sub = self.node.create_subscription(String, '/ui/guide_verbal', self.guide_verbal_callback, 10)
        self.location_name_pub = self.node.create_publisher(String, '/ui/guide_options', 10)
        self.detected_location = None
        self.verbal_guidance = None
        self.navigation_guidance = False
        self.direction_request = False
        
        self.tts = TextToSpeech()
        self.should_stop = False
        self.text_to_speech_init()
        self.classifier = LocationClassifier()
        self.classifier.initialize_process()
        self.trigerring_words = ["hi", "hello", "hey"]
        self.ending_words = ["thank you", "bye", "thanks", "thank"]
        self.triggered = False
        self.audio_queue = multiprocessing.Queue()
        self.stt_queue = multiprocessing.Queue()
        # self.sleep_queue = multiprocessing.Queue()
        self.vad_audio = VADAudio(
            aggressiveness=2,
            input_rate=32000,
            # device = 33,
        )
        self.whisper_process = multiprocessing.Process(
            target=self.start_stt,
            args=(
                self.audio_queue,
                self.stt_queue,
            ),
        )

        # self.llm_process = multiprocessing.Process(
        #     target=self.start_llm,
        #     args=(
        #         self.stt_queue,
        #         self.text_to_speech,
        #         self.text_to_speech_queue_check,
        #         self.sleep_queue,
        #     ),
        # )
        self.whisper_process.start()
        # self.llm_process.start()
    def guide_navigation_callback(self, msg):
        self.navigation_guidance = True

    def guide_verbal_callback(self, msg):
        self.verbal_guidance = msg.data

    def call_back(self, msg):
        self.triggered = True

    def start_stt(self, input_q, output_q):
        whisper = FasterWhisper("medium.en", "cuda", "float32")
        while True:
            audio_ = input_q.get()
            text = whisper.transcribe_(audio_)
            output_q.put(text)

    # def start_llm(self, input_q, tts_func, q_check_func, sleep_q):
    #     llm = LLM(
    #         m="/SSD/exllamav2_old/my_model",
    #         mode="llama",
    #         # pt=True,
    #         ncf=True,
    #         sp="You are the Smart Mobile Robot Receptionist at the Department of Electronic and Telecommunication Engineering of the University of Moratuwa. ENTC is a short form that is used to refer to the department of Electronic and Telecommunication Engineering. You have to behave like the receptionist at ENTC. Almost every question and statement is related to the department and you have to answer from the context of the department. Greet appropriately and ask how you can assist them. When ending a conversation thank the user appropriately for contacting the robot receptionist. If a user asks for directions to any location in the department, directly use the given information below related to the department. If you can not find relevant information to respond to a user question or statement, decently say you can not help and manage the situation. You have to respond to the user questions or statements delimited by triple backticks. Here is some information about the department as the receptionist you must know. The head of the department: Dr. Ranga Rodrigo. Direction to the conference room: There is the staircase. The conference room is on the first floor. Once you reach the first floor, go through the passage on your right-hand side. Then turn right, go some distance, turn left, and go toward the end of the passage. Then you can see the conference room in front of you. Direction to the staff common room: There is the staircase. The staff common room is on the first floor.  Once you reach the first floor, go through the passage on your right-hand side. Then turn right, go some distance, turn left and go toward the end of the passage, turn left and go on. Then you can see the staff common room. Direction to the seminar room: There is the staircase. The seminar room is on the third floor. Once you reach the third floor, go through the passage on your right-hand side. Then go up through the stairs ahead. Then you can see the seminar room on your lefthand side. Direction to the postgraduate room: There is the staircase. The postgraduate room is on the fourth floor. Direction to the radio room: There is the staircase. The radio room is on the rooftop of the building. Direction to the ENTC1 hall: There is the ENTC1 hall. Direction to the communication laboratory: There is the staircase. The communication laboratory is on the third floor. Once you reach the third floor, go through the passage on your right-hand side. Then turn left and go towards the end of the passage. Then you can see the communication laboratory in front of you. Direction to the computer lab: There is the staircase. The computer laboratory is on the first floor. Once you reach the first floor, you can see the computer laboratory on your left side. Direction to the head of the department's office: There is the staircase. The office of the head of the department is on the first floor. Once you reach the first floor, go through the passage on your right-hand side. Then turn right and go towards the end of the passage. Then you can see the office of the head of department on your lefthand side. Direction to the analog lab: There is the staircase. The analog electronic laboratory is on the second floor. Once you reach the second floor, go through the passage on your right-hand side. Then turn left and go towards the end of the passage. Then you can see the analog electronic laboratory on your lefthand side. Direction to the digital laboratory: There is the staircase. The digital electronic laboratory is on the second floor. Once you reach the second floor, go through the passage on your right-hand side. Then turn left and go towards the end of the passage. Then you can see the digital electronic laboratory in front of you. Direction to the department office: There is the staircase. The department office is on the first floor. Once you reach the first floor, go through the passage on your right-hand side. Then turn right and towards the end of the passage. Then you can see the department office on your lefthand side. Direction to the electronic workshop: There is the staircase. The electronic workshop is on the second floor. Once you reach the second floor, go through the passage on your right-hand side. Then turn right and go towards the end of the passage. Then you can see the electronic workshop on your lefthand side. Direction to the computer lab: There is the staircase. The soldering room is on the second floor. Once you reach the second floor, go through the passage on your right-hand side. Then turn right and go about two meters through the passage. Then you can see the soldering room on your lefthand side. ",
    #     )
    #     while True:
    #         text_input = input_q.get()
    #         llm.chat_(
    #             text_input,
    #             tts_func,
    #             q_check_func,
    #             sleep_q,
    #         )

    def start_listening(self):
        time.sleep(1)
        print("Listening ... ")
        frames = self.vad_audio.vad_collector()
        spinner = Halo(spinner="line")
        wav_data = bytearray()
        tic = time.time()
        tic2 = time.time()
        self.vad_audio.clear_queue()
        for frame in frames:
            if time.time() - tic > 120:
                return
            if self.should_stop:
                break
            if frame is not None and ((time.time()-tic2)<15):
                if spinner:
                    spinner.start()
                wav_data.extend(frame)
            else:
                tic2 = time.time()
                if spinner:
                    spinner.stop()
                name = datetime.now().strftime("savewav_%Y-%m-%d_%H-%M-%S_%f.wav")
                self.vad_audio.write_wav(
                    os.path.join(
                        "",
                        name,
                    ),
                    wav_data,
                )
                numpy_array = np.frombuffer(wav_data, dtype=np.int16)
                numpy_array = numpy_array.astype(np.float32) / 32768.0
                self.audio_queue.put(numpy_array)
                text = self.stt_queue.get()
                # text = self.whisper.transcribe_(numpy_array)
                if self.verbal_guidance is not None:
                    play_audio_clip(self.verbal_guidance)
                    self.vad_audio.clear_queue()
                    self.verbal_guidance = None
                elif self.navigation_guidance:
                    return
                elif text is not None:
                    tic = time.time()
                    text_ = text.lower()
                    for word in self.ending_words:
                        if word in text_:
                            self.triggered = True
                            self.blocking_tts(random.choice(thanking_messages))
                            self.vad_audio.clear_queue()
                            wav_data = bytearray()
                            return
                    self.detected_location, self.direction_request = self.classifier.classify_location(text)
                    if self.detected_location is not None:
                        msg = String()
                        msg.data = self.detected_location
                        self.location_name_pub.publish(msg)
                        self.detected_location = None
                    if self.verbal_guidance is not None:
                        play_audio_clip(self.verbal_guidance)
                    if not self.direction_request:
                        self.language_understanding_and_generation(text)
                    else:
                        self.blocking_tts("Let me help you with it. Please select an option from the screen.")
                    # self.stt_queue.put(text)
                    # flag = self.sleep_queue.get()
                    tic = time.time()
                self.vad_audio.clear_queue()
                wav_data = bytearray()

    def wait_idle(self):
        print("Listening ... ")
        frames = self.vad_audio.vad_collector()
        spinner = Halo(spinner="line")
        wav_data = bytearray()
        for frame in frames:
            if self.triggered:
                self.triggered = False
                return
            if frame is not None:
                if spinner:
                    spinner.start()
                wav_data.extend(frame)
            else:
                if spinner:
                    spinner.stop()
                numpy_array = np.frombuffer(wav_data, dtype=np.int16)
                numpy_array = numpy_array.astype(np.float32) / 32768.0
                text = self.whisper.transcribe_(numpy_array)
                if text is not None:
                    text = text.lower()
                    for word in self.trigerring_words:
                        if word in text:
                            self.triggered = True
                            self.vad_audio.clear_queue()
                            wav_data = bytearray()
                            return
                self.vad_audio.clear_queue()
                wav_data = bytearray()

    def text_to_speech_init(self):
        # self.recorded_audio = multiprocessing.Queue()
        # self.tts_process = multiprocessing.Process(
        #     target=self.tts.t, args=(self.generated_text, self.shared_mem)
        # )
        # self.tts_process.start()
        # self.tts_process.join()
        self.tts.initialize_processes()

    def kill_text_to_speech(self):
        # if self.tts_process.is_alive():
        #     self.tts_process.kill()
        self.tts.kill_processess()

    def kill_classification(self):
        self.classifier.kill_processes()

    def kill_whisper(self):
        if self.whisper_process.is_alive():
            self.whisper_process.kill()

    # def kill_llm(self):
    #     if self.llm_process.is_alive():
    #         self.llm_process.kill()

    def language_understanding_and_generation(self, text, warmup_flag=False):
        self.llm.chat_(text, self.text_to_speech, self.tts_end, warmup_flag)

    def text_to_speech(self, text):
        self.tts.convert_text_to_speech(text)

    def text_to_speech_queue_check(self):
        self.tts.check_output_queue()

    def tts_end(self):
        self.tts.get_ending()

    def blocking_tts(self, text):
        self.text_to_speech(text)
        self.text_to_speech("$$")
        i = self.tts_end()
        return

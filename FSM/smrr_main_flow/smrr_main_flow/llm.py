from pickle import STRING
import sys, os, time, math
import pygame
import subprocess

# from google.cloud import texttospeech

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.append("/SSD/exllamav2_old/examples")
sys.path.append("/SSD/exllamav2_old")

from exllamav2 import (
    ExLlamaV2,
    ExLlamaV2Config,
    ExLlamaV2Cache,
    ExLlamaV2Cache_8bit,
    ExLlamaV2Tokenizer,
    model_init,
)

import argparse
import torch

from exllamav2.generator import ExLlamaV2StreamingGenerator, ExLlamaV2Sampler

from chat_formatting import CodeBlockFormatter
from chat_prompts import prompt_formats

prompt_formats_list = list(prompt_formats.keys())


class Args:
    pass


# Options
class LLM:
    args = Args()

    def __init__(
        self,
        m,
        dm=None,  # model directory`
        nds=False,
        modes=False,
        mode="llama",
        un="User",
        bn="Chatbot",
        sp=None,
        # p=None,
        temp=0.95,
        smooth=0.0,
        dyntemp=None,
        topk=50,
        topp=0.8,
        topa=0.0,
        typical=0.0,
        repp=1.05,
        freqpen=0.0,
        prespen=0.0,
        maxr=1000,
        resc=250,
        ncf=False,
        c8=False,
        pt=False,
        amnesia=False,
        gs=None,
        l=None,
        rs=None,
        ra=None,
        nfa=False,
        lm=False,
        ept=None,
        fst=False,
    ):
        self.args.model_dir = m
        self.args.draft_model_dir = dm
        self.args.no_draft_scale = nds
        self.args.modes = modes
        self.args.mode = mode
        self.args.username = un
        self.args.botname = bn
        self.args.system_prompt = sp
        # self.args.p = p
        self.args.temperature = temp
        self.args.smoothing_factor = smooth
        self.args.dynamic_temperature = dyntemp
        self.args.top_k = topk
        self.args.top_p = topp
        self.args.top_a = topa
        self.args.typical = typical
        self.args.repetition_penalty = repp
        self.args.frequency_penalty = freqpen
        self.args.presence_penalty = prespen
        self.args.max_response_tokens = maxr
        self.args.response_chunk = resc
        self.args.no_code_formatting = ncf
        self.args.cache_8bit = c8
        self.args.print_timings = pt
        self.args.amnesia = amnesia
        self.args.gpu_split = gs
        self.args.length = l
        self.args.rope_scale = rs
        self.args.rope_alpha = ra
        self.args.no_flash_attn = nfa
        self.args.low_mem = lm
        self.args.experts_per_token = ept
        self.args.fast_safetensors = fst

        if self.args.modes:
            print(" -- Available formats:")
            for k, v in prompt_formats.items():
                print(f" --   {k:12} : {v().description}")
            sys.exit()

        self.username = self.args.username
        self.botname = self.args.botname
        self.system_prompt = self.args.system_prompt

        if self.args.mode is None:
            print(" ## Error: No mode specified.")
            sys.exit()

        self.prompt_format = prompt_formats[self.args.mode]()
        self.prompt_format.botname = self.botname
        self.prompt_format.username = self.username
        if self.system_prompt is None:
            self.system_prompt = self.prompt_format.default_system_prompt()

        # Initialize model and tokenizer

        model_init.check_args(self.args)
        model_init.print_options(self.args)
        self.model, self.tokenizer = model_init.init(self.args, allow_auto_split=True)

        # Initialize draft model if provided, assume it always fits on first device

        self.draft_model = None
        self.draft_cache = None

        if self.args.draft_model_dir:

            print(f" -- Draft model: {self.args.draft_model_dir}")

            self.draft_config = ExLlamaV2Config()
            self.draft_config.model_dir = self.args.draft_model_dir
            self.draft_config.prepare()

            if self.draft_config.max_seq_len < self.model.config.max_seq_len:

                if self.args.no_draft_scale:
                    print(
                        f" !! Warning: Draft model native max sequence length is less than sequence length for model. Speed may decrease after {self.draft_config.max_seq_len} tokens."
                    )
                else:
                    ratio = (
                        self.model.config.max_seq_len / self.draft_config.max_seq_len
                    )
                    alpha = -0.13436 + 0.80541 * ratio + 0.28833 * ratio**2
                    self.draft_config.scale_alpha_value = alpha
                    print(f" -- Applying draft model RoPE alpha = {alpha:.4f}")

            self.draft_config.max_seq_len = self.model.config.max_seq_len
            self.draft_config.no_flash_attn = self.args.no_flash_attn
            self.draft_config.scale_pos_emb = self.args.rope_scale

            print(" -- Loading draft model...")

            self.draft_model = ExLlamaV2(self.draft_config)
            self.draft_model.load()

            if self.args.cache_8bit:
                draft_cache = ExLlamaV2Cache_8bit(self.draft_model)
            else:
                draft_cache = ExLlamaV2Cache(self.draft_model)

        # Create cache

        if self.args.cache_8bit:
            cache = ExLlamaV2Cache_8bit(self.model, lazy=not self.model.loaded)
        else:
            cache = ExLlamaV2Cache(self.model, lazy=not self.model.loaded)

        # Load model now if auto split enabled

        if not self.model.loaded:

            print(" -- Loading model...")
            self.model.load_autosplit(cache)

            # Generator

        self.generator = ExLlamaV2StreamingGenerator(
            self.model, cache, self.tokenizer, self.draft_model, self.draft_cache
        )

        self.settings = ExLlamaV2Sampler.Settings()
        self.settings.temperature = self.args.temperature
        self.settings.top_k = self.args.top_k
        self.settings.top_p = self.args.top_p
        self.settings.top_a = self.args.top_a
        self.settings.typical = self.args.typical
        self.settings.token_repetition_penalty = self.args.repetition_penalty
        self.settings.token_frequency_penalty = self.args.frequency_penalty
        self.settings.token_presence_penalty = self.args.presence_penalty
        self.settings.smoothing_factor = self.args.smoothing_factor

        if self.args.dynamic_temperature:
            dt_args = [
                float(alloc) for alloc in self.args.dynamic_temperature.split(",")
            ]
            self.settings.min_temp = dt_args[0]
            self.settings.max_temp = dt_args[1]
            self.settings.temp_exponent = dt_args[2]

        self.max_response_tokens = self.args.max_response_tokens
        self.min_space_in_context = self.args.response_chunk

        # Stop conditions

        self.generator.set_stop_conditions(
            self.prompt_format.stop_conditions(self.tokenizer)
        )

        # ANSI color codes

        self.col_default = "\u001b[0m"
        self.col_user = "\u001b[33;1m"  # Yellow
        self.col_bot = "\u001b[34;1m"  # Blue
        self.col_error = "\u001b[31;1m"  # Magenta
        self.col_sysprompt = "\u001b[37;1m"  # Grey

        # Code block formatting

        self.codeblock_formatter = (
            None if self.args.no_code_formatting else CodeBlockFormatter()
        )
        self.in_code_block = False

        self.delim_overflow = ""

        # Other options

        self.print_timings = self.args.print_timings
        self.amnesia = self.args.amnesia

        # Main loop

    # Chat context

    def format_prompt(self, user_prompt, first):
        # global self.system_prompt, self.prompt_format

        if first:
            return (
                self.prompt_format.first_prompt()
                .replace("<|system_prompt|>", self.system_prompt)
                .replace("<|user_prompt|>", user_prompt)
            )
        else:
            return self.prompt_format.subs_prompt().replace(
                "<|user_prompt|>", user_prompt
            )

    def encode_prompt(self, text):
        # global tokenizer, prompt_format

        add_bos, add_eos, encode_special_tokens = self.prompt_format.encoding_options()
        return self.tokenizer.encode(
            text,
            add_bos=add_bos,
            add_eos=add_eos,
            encode_special_tokens=encode_special_tokens,
        )

    user_prompts = []
    responses_ids = []

    def get_tokenized_context(self, max_len):
        # global user_prompts, responses_ids

        while True:

            context = torch.empty((1, 0), dtype=torch.long)

            for turn in range(len(self.user_prompts)):

                up_text = self.format_prompt(
                    self.user_prompts[turn], context.shape[-1] == 0
                )
                up_ids = self.encode_prompt(up_text)
                context = torch.cat([context, up_ids], dim=-1)

                if turn < len(self.responses_ids):
                    context = torch.cat([context, self.responses_ids[turn]], dim=-1)

            if context.shape[-1] < max_len:
                return context

            # If the context is too long, remove the first Q/A pair and try again. The system prompt will be moved to
            # the first entry in the truncated context

            self.user_prompts = self.user_prompts[1:]
            self.responses_ids = self.responses_ids[1:]

    def make_sentence(self, chunk_, flag, funct, funct2, qu):
        if flag:
            # return self.model_responses
            self.model_responses.append(self.model_response)
            # self.text_to_speech_with_pitch(
            #     self.model_response, 2, "output_with_pitch.wav"
            # )
            # funct(self.model_response)
            # print(self.model_response)
            self.model_response = ""
            while True:
                if funct2:
                    break
            qu.put(True)
            return
        elif chunk_ in [".", ",", "?", "!"]:
            self.model_response += chunk_
            self.model_responses.append(self.model_response)
            # self.text_to_speech_with_pitch(
            #     self.model_response, 2, "output_with_pitch.wav"
            # )
            # # print(self.model_response)
            # self.model_response = ""
            funct(self.model_response)
            self.model_response = ""
            return
        else:
            self.model_response += chunk_
            return

    # def text_to_speech_with_pitch(self, text,p,o):
    #         """Synthesizes speech from the input string of text."""
    #         client = texttospeech.TextToSpeechClient.from_service_account_file("key.json")

    #         input_text = texttospeech.SynthesisInput(text=text)

    #         # Note: the voice can also be specified by name.
    #         # Names of voices can be retrieved with client.list_voices().
    #         voice = texttospeech.VoiceSelectionParams(
    #             language_code="en-GB",
    #             name="en-GB-Neural2-C",
    #             ssml_gender=texttospeech.SsmlVoiceGender.FEMALE,
    #         )

    #         audio_config = texttospeech.AudioConfig(
    #             audio_encoding=texttospeech.AudioEncoding.MP3
    #         )

    #         response = client.synthesize_speech(
    #             request={"input": input_text, "voice": voice, "audio_config": audio_config}
    #         )

    #         # The response's audio_content is binary.
    #         with open("output.mp3", "wb") as out:
    #             out.write(response.audio_content)
    #             # print('Audio content written to file "output.mp3"')
    #         self.play_wav("output.mp3")

    def text_to_speech_with_pitch(self, text, p, o):
        # Example command to run
        text = text.strip('"')
        command = (
            f"""echo '(SayText "{text}")' | /SSD/build/festival/bin/festival --pipe"""
        )
        # command = f"echo '(SayText \"{text.replace('\"', '\\"')}\")' | /SSD/build/festival/bin/festival --pipe"

        # Run the command using subprocess
        result = subprocess.run(
            command,
            shell=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
        )

        # Print the output
        # print("Output:", result.stdout)

        # Print any errors, if occurred
        if result.stderr:
            print("Error:", result.stderr)

    def play_wav(self, file_path):

        # while True:
        #     if self.can_play:
        #         self.can_play=False
        pygame.init()
        pygame.mixer.init()

        try:
            # Load the WAV file
            sound = pygame.mixer.Sound(file_path)

            # Play the sound
            sound.play()

            # Wait for the sound to finish playing
            # tic=time.time()
            pygame.time.wait(int(sound.get_length() * 1000))
            # print("time", time.time()-tic)
        except pygame.error as e:
            print(f"Error playing audio: {e}")
        finally:
            pygame.quit()
            # self.can_play=True
            return

    def chat_(self, user_p, tts_func, q_check_func, slp_q):
        # while True:
        # self.engine = pyttsx3.init()
        # pygame.init()
        # pygame.mixer.init()
        # Get user prompt
        self.can_write = True
        self.can_play = True
        print()
        # up = input(col_user + username + ": " + col_default).strip()
        up = user_p
        print()

        # Add to context
        # global user_prompts, responses_ids, in_code_block
        self.user_prompts.append(up)

        # Send tokenized context to generator

        self.active_context = self.get_tokenized_context(
            self.model.config.max_seq_len - self.min_space_in_context
        )
        self.generator.begin_stream(self.active_context, self.settings)

        # Stream response

        if self.prompt_format.print_bot_name():

            print(self.col_bot + self.botname + ": " + self.col_default, end="")

        self.response_tokens = 0
        self.response_text = ""
        self.responses_ids.append(torch.empty((1, 0), dtype=torch.long))

        if self.print_timings:
            time_begin_stream = time.time()
            if self.draft_model is not None:
                self.generator.reset_sd_stats()

        self.model_response = ""
        self.model_responses = []
        temp = []
        while True:

            # Get response stream
            # print("----mod-res", self.model_response, "--------")
            chunk, eos, tokens = self.generator.stream()
            if len(self.response_text) == 0:
                chunk = chunk.lstrip()
            self.response_text += chunk
            self.responses_ids[-1] = torch.cat([self.responses_ids[-1], tokens], dim=-1)

            # Check for code block delimiters
            # Let formatter suppress text as long as it may be part of delimiter
            chunk, codeblock_delimiter = (
                (chunk, False)
                if self.codeblock_formatter is None
                else self.codeblock_formatter.process_delimiter(chunk)
            )

            # Enter code block
            if not self.in_code_block:

                # Start of codeblock
                if codeblock_delimiter:
                    self.codeblock_formatter.begin()
                    print("\n")
                    self.in_code_block = True
                    codeblock_delimiter = False

            # Print

            if self.in_code_block:

                # Print unformatted
                print("3232")
                # self.make_sentence(chunk, eos)
                self.codeblock_formatter.print_code_block(chunk)

            else:

                # Print formatted
                print(chunk, end="")
                self.make_sentence(chunk, eos, tts_func, q_check_func, slp_q)
                # temp.append(chunk)
                # pass

            # Exit code block

            if self.in_code_block:

                # End of code block
                if codeblock_delimiter:

                    # Edge case when we get EOS right after code block
                    if eos:
                        self.codeblock_formatter.print_code_block("\n")

                    print("\033[0m")  # Reset block color to be certain
                    self.in_code_block = False
                    codeblock_delimiter = False

            sys.stdout.flush()
            # time.sleep(1)

            # If model has run out of space, rebuild the context and restart stream

            if self.generator.full():

                active_context = self.get_tokenized_context(
                    self.model.config.max_seq_len - self.min_space_in_context
                )
                self.generator.begin_stream(active_context, self.settings)

            # If response is too long, cut it short, and append EOS if that was a stop condition

            self.response_tokens += 1
            if self.response_tokens == self.max_response_tokens:

                if self.tokenizer.eos_token_id in self.generator.stop_tokens:
                    responses_ids[-1] = torch.cat(
                        [
                            responses_ids[-1],
                            self.tokenizer.single_token(self.tokenizer.eos_token_id),
                        ],
                        dim=-1,
                    )

                print()
                print(
                    self.col_error
                    + f" !! Response exceeded {self.max_response_tokens} tokens and was cut short."
                    + self.col_default
                )
                break

            # EOS signal returned

            if eos:

                if self.prompt_format.print_extra_newline():
                    print()
                break

        # Prompt timings
        # print(temp)
        if self.print_timings:

            time_end_stream = time.time()
            speed = self.response_tokens / (time_end_stream - time_begin_stream)

            if self.draft_model is not None:
                eff, acc, _, _, _ = self.generator.get_sd_stats()
                sd_stats = f", SD eff. {eff*100:.2f}%, SD acc. {acc*100:.2f}%"
            else:
                sd_stats = ""

            print()
            print(
                self.col_sysprompt
                + f"(Response: {self.response_tokens} tokens, {speed:.2f} tokens/second{sd_stats})"
                + self.col_default
            )

        # Optionally forget context after each response

        if self.amnesia:
            user_prompts = []
            responses_ids = []


# chat_(args.prompt)

# mistral = LLM(
#     m="my_model",
#     mode="llama",
#     # pt=True,
#     ncf=True,
#     sp="You are the Smart Mobile Robot Receptionist at the Department of Electronic and Telecommunication Engineering of the University of Moratuwa. ENTC is a short form that is used to refer to the department of Electronic and Telecommunication Engineering. You have to behave like the receptionist at ENTC. Almost every question and statement is related to the department and you have to answer from the context of the department. Greet appropriately and ask how you can assist them. When ending a conversation thank the user appropriately for contacting the robot receptionist. If a user asks for directions to any location in the department, directly use the given information below related to the department. If you can not find relevant information to respond to a user question or statement, decently say you can not help and manage the situation. You have to respond to the user questions or statements delimited by triple backticks. Here is some information about the department as the receptionist you must know. The head of the department: Dr. Ranga Rodrigo. Direction to the conference room: There is the staircase. The conference room is on the first floor. Once you reach the first floor, go through the passage on your right-hand side. Then turn right, go some distance, turn left, and go toward the end of the passage. Then you can see the conference room in front of you. Direction to the staff common room: There is the staircase. The staff common room is on the first floor.  Once you reach the first floor, go through the passage on your right-hand side. Then turn right, go some distance, turn left and go toward the end of the passage, turn left and go on. Then you can see the staff common room. Direction to the seminar room: There is the staircase. The seminar room is on the third floor. Once you reach the third floor, go through the passage on your right-hand side. Then go up through the stairs ahead. Then you can see the seminar room on your lefthand side. Direction to the postgraduate room: There is the staircase. The postgraduate room is on the fourth floor. Direction to the radio room: There is the staircase. The radio room is on the rooftop of the building. Direction to the ENTC1 hall: There is the ENTC1 hall. Direction to the communication laboratory: There is the staircase. The communication laboratory is on the third floor. Once you reach the third floor, go through the passage on your right-hand side. Then turn left and go towards the end of the passage. Then you can see the communication laboratory in front of you. Direction to the computer lab: There is the staircase. The computer laboratory is on the first floor. Once you reach the first floor, you can see the computer laboratory on your left side. Direction to the head of the department's office: There is the staircase. The office of the head of the department is on the first floor. Once you reach the first floor, go through the passage on your right-hand side. Then turn right and go towards the end of the passage. Then you can see the office of the head of department on your lefthand side. Direction to the analog lab: There is the staircase. The analog electronic laboratory is on the second floor. Once you reach the second floor, go through the passage on your right-hand side. Then turn left and go towards the end of the passage. Then you can see the analog electronic laboratory on your lefthand side. Direction to the digital laboratory: There is the staircase. The digital electronic laboratory is on the second floor. Once you reach the second floor, go through the passage on your right-hand side. Then turn left and go towards the end of the passage. Then you can see the digital electronic laboratory in front of you. Direction to the department office: There is the staircase. The department office is on the first floor. Once you reach the first floor, go through the passage on your right-hand side. Then turn right and towards the end of the passage. Then you can see the department office on your lefthand side. Direction to the electronic workshop: There is the staircase. The electronic workshop is on the second floor. Once you reach the second floor, go through the passage on your right-hand side. Then turn right and go towards the end of the passage. Then you can see the electronic workshop on your lefthand side. Direction to the computer lab: There is the staircase. The soldering room is on the second floor. Once you reach the second floor, go through the passage on your right-hand side. Then turn right and go about two meters through the passage. Then you can see the soldering room on your lefthand side. ",
# )


# while True:
#     promp = input("User :")
#     mistral.chat_(promp)

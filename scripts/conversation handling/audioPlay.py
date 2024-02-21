import pygame


def play_wav(file_path):
    pygame.init()
    pygame.mixer.init()

    try:
        # Load the WAV file
        sound = pygame.mixer.Sound(file_path)

        # Play the sound
        sound.play()

        # Wait for the sound to finish playing
        pygame.time.wait(int(sound.get_length() * 1000))

    except pygame.error as e:
        print(f"Error playing audio: {e}")
    finally:
        pygame.quit()


# Replace 'your_audio_file.wav' with the actual path to your WAV file
audio_file_path = "audio.wav"

play_wav(audio_file_path)

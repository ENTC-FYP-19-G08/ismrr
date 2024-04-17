import subprocess

# audio_clips={}

def play_audio_clip(location):
    # global audio_clips
    command = f"""aplay /SSD/direction_audio/{location}.wav"""
    # command = f"""aplay {location_file_path}"""
    result = subprocess.run(
        command,
        shell=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    return



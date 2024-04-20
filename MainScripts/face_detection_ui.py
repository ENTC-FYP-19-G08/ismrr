import subprocess
import time
import psutil
import sys
import signal

def signal_handler(sig, frame):
    print('You pressed Ctrl+C!')
    kill_ros_processes()
    sys.exit(0)

def kill_ros_processes():
    # Get a list of all running processes
    for process in psutil.process_iter(['pid', 'name']):
        try:
            # Check if the process name contains 'ros'
            # print(str(process.cmdline()))
            if 'ros' in str(process.cmdline()):
                # Terminate the process
                print(f"Terminating process: {process.info['name']} (PID: {process.pid})")
                process.terminate()
        except (psutil.NoSuchProcess, psutil.AccessDenied, psutil.ZombieProcess):
            pass

# Example: List files in the current directory
kill_ros_cmd = "kill_ros".split()
zed_cmd = "ros2 launch zed_wrapper zed_camera.launch.py camera_model:=zed2".split()
face_recog_cmd = "ros2 run face_recognition detect_face".split()
flow_cmd = "python3 /SSD/ros2_ws/src/FSM/smrr_main_flow/smrr_main_flow/flow_node.py".split()
ui_cmd = "ros2 run  smrr_ui smrr_ui".split()
processes = []

signal.signal(signal.SIGINT, signal_handler)

try:
    # kill_ros = subprocess.Popen(kill_ros_cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    # processes.append(kill_ros)
    # time.sleep(4)
    zed = subprocess.Popen(zed_cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    processes.append(zed)
    time.sleep(5)
    face_recog = subprocess.Popen(face_recog_cmd)
    processes.append(face_recog)
    time.sleep(1)
    ui = subprocess.Popen(ui_cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    processes.append(ui)
    time.sleep(1)
    flow = subprocess.Popen(flow_cmd)
    processes.append(flow)
    time.sleep(2)
    print("All subprocesses started.")
    while processes:
        for proc in processes:
            # Check if process has ended and remove it from the list if it has
            if proc.poll() is not None:
                processes.remove(proc)
        time.sleep(0.1)  # Sleep briefly to reduce CPU usag

 

except :
    print("Terminating subprocesses...")
    kill_ros_processes()
    print("All subprocesses terminated.")

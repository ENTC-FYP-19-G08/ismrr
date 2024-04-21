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
amcl_cmd = "bash -i -c amcl".split()
flow_cmd = "python3 /SSD/ros2_ws/src/FSM/smrr_main_flow/smrr_main_flow/DEMO_NAVIGATION.py".split()
processes = []

signal.signal(signal.SIGINT, signal_handler)

try:
    # kill_ros = subprocess.Popen(kill_ros_cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    # processes.append(kill_ros)
    # time.sleep(4)
    amcl = subprocess.Popen(amcl_cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    processes.append(amcl)
    time.sleep(20)
    flow = subprocess.Popen(flow_cmd)
    processes.append(flow)
    # time.sleep(2)
   
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

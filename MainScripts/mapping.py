import subprocess
import time

import psutil

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
        except:
            pass

# Example: List files in the current directory
bringup_cmd = "ros2 launch smrr_bringup smrr_bringup_launch.py".split()
navigation_cmd = "ros2 launch nav2_bringup navigation_launch.py".split()
arduino_serial_cmd = "ros2 run smrr_arduino_serial arduino_serial".split()
teleop_cmd = "ros2 launch tele_op teleop_launch.py".split()
collison_monitor_cmd = "ros2 run collison_monitor collison_monitor".split()
slam_cmd = "ros2 launch slam_toolbox online_async_launch.py".split()
rviz_cmd = "rviz2".split()

processes = []

try:
    rviz = subprocess.Popen(rviz_cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    processes.append(rviz)
    # navigation = subprocess.Popen(navigation_cmd)
    # processes.append(navigation)
    # time.sleep(2)
    bringup = subprocess.Popen(bringup_cmd)
    processes.append(bringup)
    time.sleep(15)
    arduino_serial = subprocess.Popen(arduino_serial_cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    processes.append(arduino_serial)
    time.sleep(1)
    teleop = subprocess.Popen(teleop_cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    processes.append(teleop)  
    time.sleep(1)
    collison_monitor = subprocess.Popen(collison_monitor_cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    processes.append(collison_monitor_cmd)
    time.sleep(1)

    slam = subprocess.Popen(slam_cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    processes.append(slam)

    print("All subprocesses started.", flush = True)
    while processes:
        for proc in processes:
            # Check if process has ended and remove it from the list if it has
            if proc.poll() is not None:
                processes.remove(proc)

        time.sleep(1)  # Sleep briefly to reduce CPU usage



except:
    print("Ctrl+C detected, terminating subprocesses...")
    #kill_ros_processes()
    print("All subprocesses terminated.", flush = True)

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
        except (psutil.NoSuchProcess, psutil.AccessDenied, psutil.ZombieProcess):
            pass

# Example: List files in the current directory
bringup_cmd = "ros2 launch smrr_bringup smrr_bringup_launch.py".split()
navigation_cmd = "ros2 launch nav2_bringup navigation_launch.py".split()
amcl_cmd = "ros2 launch nav2_bringup localization_launch.py map:=/SSD/ros2_ws/src/Maps/entc_ground_cut.yaml".split()
rviz_cmd = "rviz2 -d /SSD/ros2_ws/src/robot_bringup/nav2/nav2_bringup/bringup/rviz/nav2_default_view.rviz".split()
serial_cmd = "ros2 run smrr_arduino_serial arduino_serial".split()
zed_cmd = "ros2 launch zed_wrapper zed_camera.launch.py camera_model:=zed2".split()
zed_scanner_cmd = "ros2 launch smrr_zed_scanner zed_scanner_launch.py"
processes = []

try:
    rviz = subprocess.Popen(rviz_cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    processes.append(rviz)
    zed = subprocess.Popen(zed_cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    processes.append(zed)
    time.sleep(5)
    zed_scanner = subprocess.Popen(zed_scanner_cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    processes.append(zed_scanner)
    navigation = subprocess.Popen(navigation_cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    processes.append(navigation)
    time.sleep(5)
    bringup = subprocess.Popen(bringup_cmd)
    processes.append(bringup)
    time.sleep(10)
    # serial = subprocess.Popen(serial_cmd)
    # processes.append(serial) 
    amcl = subprocess.Popen(amcl_cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    processes.append(amcl)

    print("All subprocesses started.")
    while processes:
        for proc in processes:
            # Check if process has ended and remove it from the list if it has
            if proc.poll() is not None:
                processes.remove(proc)
        time.sleep(0.1)  # Sleep briefly to reduce CPU usage

except KeyboardInterrupt:
    print("Ctrl+C detected, terminating subprocesses...")
    kill_ros_processes()
    print("All subprocesses terminated.")

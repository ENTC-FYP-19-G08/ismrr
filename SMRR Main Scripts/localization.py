import subprocess
import time
# Example: List files in the current directory
bringup_cmd = "ros2 launch smrr_bringup smrr_bringup_launch.py".split()
navigation_cmd = "ros2 launch nav2_bringup navigation_launch.py".split()
amcl_cmd = "ros2 launch nav2_bringup localization_launch.py map:=/home/jetson/uav_lidar_cut_sync.yaml".split()
rviz_cmd = "rviz2".split()
zed_cmd = "ros2 launch zed_wrapper zed_camera.launch.py camera_model:=zed2".split()

processes = []

try:
    rviz = subprocess.Popen(rviz_cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    processes.append(rviz)
    bringup = subprocess.Popen(bringup_cmd)
    processes.append(bringup)
    time.sleep(15)
    # navigation = subprocess.Popen(navigation_cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    # processes.append(navigation)
    # time.sleep(2)
    amcl = subprocess.Popen(amcl_cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    processes.append(amcl)
    time.sleep(5)
    zed = subprocess.Popen(zed_cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    processes.append(zed)

    print("All subprocesses started.")
    while processes:
        for proc in processes:
            # Check if process has ended and remove it from the list if it has
            if proc.poll() is not None:
                processes.remove(proc)
        time.sleep(10)  # Sleep briefly to reduce CPU usage

except KeyboardInterrupt:
    print("Ctrl+C detected, terminating subprocesses...")
    # Attempt to terminate all subprocesses gracefully
    for proc in processes:
        proc.terminate()  # Sends SIGTERM signal
        try:
            # Force terminate 
            proc.kill()
        except:
            continue
    print("All subprocesses terminated.")

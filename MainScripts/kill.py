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

if __name__ == "__main__":
    kill_ros_processes()

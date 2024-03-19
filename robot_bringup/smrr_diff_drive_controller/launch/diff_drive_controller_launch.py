import os

from launch import LaunchDescription
from launch_ros.actions import Node
# from launch_ros.descriptions import ParameterFile
from ament_index_python.packages import get_package_share_directory

    
def generate_launch_description():
    
    pkg_dir = get_package_share_directory("smrr_diff_drive_controller")

    diff_node = Node(
            package='smrr_diff_drive_controller',
            # namespace='turtlesim1',
            executable='diff_drive_controller',
            name='diff_drive_controller_node',
            output="screen",
            parameters=[os.path.join(pkg_dir, 'config', 'params.yaml')],
            # prefix=['xterm -e gdb -ex run --args']
        # arguments=['--ros-args', '--log-level', 'debug'],
        #emulate_tty=True)
            
        )


    serial_node = Node(
            package='smrr_arduino_serial',
            # namespace='turtlesim1',
            executable='arduino_serial',
            name='arduino_serial_node',
            output="screen",
            # prefix=['xterm -e gdb -ex run --args']
        # arguments=['--ros-args', '--log-level', 'debug'],
        #emulate_tty=True)
            
        )
   
    
    ld = LaunchDescription()
    ld.add_action(serial_node)
    ld.add_action(diff_node)

    
    return ld

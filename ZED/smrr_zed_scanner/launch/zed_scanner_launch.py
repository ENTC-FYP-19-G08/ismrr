import os

from launch import LaunchDescription
from launch_ros.actions import Node
# from launch_ros.descriptions import ParameterFile
from ament_index_python.packages import get_package_share_directory

    
def generate_launch_description():
    
    pkg_dir = get_package_share_directory("smrr_zed_scanner")

    zed_scanner_node = Node(
            package='smrr_zed_scanner',
            # namespace='turtlesim1',
            executable='zed_scanner',
            name='smrr_zed_scanner_node',
            output="screen",
            parameters=[os.path.join(pkg_dir, 'config', 'params.yaml')],
            # prefix=['xterm -e gdb -ex run --args']
        # arguments=['--ros-args', '--log-level', 'debug'],
        #emulate_tty=True)
            
        )
   
    
    ld = LaunchDescription()
    ld.add_action(zed_scanner_node)

    
    return ld

import os

from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
    
def generate_launch_description():
    
    pkg_dir = get_package_share_directory("lidar_cutter")
 
    start_lidar_cutter_node = Node(
            package='lidar_cutter',
            executable='exec_lidar_cutter',
            name='lidar_cutter_node',
            output="screen",
            parameters=[os.path.join(pkg_dir, 'config', 'params.yaml')],
            # prefix=['xterm -e gdb -ex run --args']
            # arguments=['--ros-args', '--log-level', 'debug'],
            # emulate_tty=True,
        )
    
    ld = LaunchDescription()
    ld.add_action(start_lidar_cutter_node)
    

    
    return ld
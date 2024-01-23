import os
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument, ExecuteProcess, IncludeLaunchDescription, TimerAction
from launch.conditions import IfCondition
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PythonExpression
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():

    diff_drive_package_dir = get_package_share_directory('smrr_diff_drive_controller')
    rplidar_package_dir = get_package_share_directory('rplidar_ros2')
    teleop_package_dir = get_package_share_directory('tele_op')

    start_diff_drive_cmd = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(os.path.join(diff_drive_package_dir, 'launch', 'diff_drive_controller_launch.py')),
        # launch_arguments={
        #                     'namespace': namespace,
        #                     'use_sim_time': use_sim_time,
        #                     # 'use_composition': 'True',
        #                     }.items()
    )
    
    start_rplidar_cmd = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(os.path.join(rplidar_package_dir, 'launch', 'rplidar_launch.py')),
        # launch_arguments={
        #                     'namespace': namespace,
        #                     'use_sim_time': use_sim_time,
        #                     # 'use_composition': 'True',
        #                     }.items()
    )

    start_teleop_cmd = IncludeLaunchDescription(PythonLaunchDescriptionSource(os.path.join(teleop_package_dir, 'launch', 'teleop_launch.py')),
        # launch_arguments={
        #                     'namespace': namespace,
        #                     'use_sim_time': use_sim_time,
        #                     # 'use_composition': 'True',
        #                     }.items()
    )


    static_tf_1 = Node(
        package = "tf2_ros", 
        executable = "static_transform_publisher",
        name = "laser_invert_tf_publisher",
        arguments =  ["0", "0", "0", "3.14159265358", "0", "0", "laser_raw", "laser"])


    static_tf_2 = Node(
        package = "tf2_ros", 
        executable = "static_transform_publisher",
        name = "laser_tf_publisher",
        arguments =  ["0.235", "0", "0", "-3.14159265358", "0", "0", "base_footprint", "laser_raw"])  
    

    static_tf_3 = Node(
        package = "tf2_ros", 
        executable = "static_transform_publisher",
        name = "link_footprint_tf_publisher",
        arguments =  ["0", "0", "0", "0", "0", "0", "base_link", "base_footprint"])      
    # Create the launch description and populate
    ld = LaunchDescription()

  
    # Declare the launch options
    ld.add_action(TimerAction(period=4.0, actions=[start_rplidar_cmd]))
    ld.add_action(start_diff_drive_cmd)
    ld.add_action(TimerAction(period=6.0,actions=[start_teleop_cmd]))
    ld.add_action(static_tf_1)
    ld.add_action(static_tf_2)
    ld.add_action(static_tf_3)
    

    return ld

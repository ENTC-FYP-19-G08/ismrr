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

    bringup_package_dir = get_package_share_directory('smrr_bringup')
    nav2_bringup_package_dir = get_package_share_directory('nav2_bringup')


    rviz_config_file = LaunchConfiguration('rviz_config_file')

    declare_rviz_config_file_cmd = DeclareLaunchArgument(
        'rviz_config_file',
        default_value=os.path.join(os.path.dirname(os.getcwd()), 'rviz', 'nav2_custom_view.rviz'),
        description='Full path to the RVIZ config file to use')
    

    robot_bringup_cmd = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(os.path.join(bringup_package_dir, 'launch', 'smrr_bringup_launch.py')),
        # launch_arguments={
        #                     'namespace': namespace,
        #                     'use_sim_time': use_sim_time,
        #                     # 'use_composition': 'True',
        #                     }.items()
    )
    
    start_amcl_cmd = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(os.path.join(nav2_bringup_package_dir, 'launch', 'localization_launch.py')),
        # launch_arguments={
        #                     'namespace': namespace,
        #                     'use_sim_time': use_sim_time,
        #                     # 'use_composition': 'True',
        #                     }.items()
    )

    
    start_navigation_cmd = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(os.path.join(nav2_bringup_package_dir, 'launch', 'navigation_launch.py')),
        # launch_arguments={
        #                     'namespace': namespace,
        #                     'use_sim_time': use_sim_time,
        #                     # 'use_composition': 'True',
        #                     }.items()
    )

    rviz_cmd = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(os.path.join(bringup_package_dir, 'launch', 'rviz_launch.py')),
        condition=IfCondition("True"),
        launch_arguments={'namespace': '',
                          'use_namespace': 'False',
                          'rviz_config': rviz_config_file}.items())
    

    ld = LaunchDescription()
  
    # Declare the launch options
    ld.add_action(declare_rviz_config_file_cmd)
    ld.add_action(rviz_cmd)
    ld.add_action(robot_bringup_cmd)
    ld.add_action(TimerAction(period=5.0,actions=[start_slam_cmd]))
    ld.add_action(TimerAction(period=8.0,actions=[start_navigation_cmd]))

    return ld

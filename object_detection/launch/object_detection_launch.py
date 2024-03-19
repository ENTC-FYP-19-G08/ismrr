from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument, ExecuteProcess, IncludeLaunchDescription, TimerAction

def generate_launch_description():

    fbl_node = Node(
        package='firebase_listener',
        # namespace='turtlesim1',
        executable='firebase_listener',
        name='firebase_listener_node',
        # namespace=namespace,
        output="screen",
        
    # arguments=['--ros-args', '--log-level', 'debug'],
    #emulate_tty=True)
    )

    fbs_node = Node(
        package='firebase_sender',
        # namespace='turtlesim1',
        executable='firebase_sender',
        name='firebase_sender_node',
        # namespace=namespace,
        output="screen",
        
    # arguments=['--ros-args', '--log-level', 'debug'],
    #emulate_tty=True)
    )

    tele_op_node = Node(
        package='tele_op',
        # namespace='turtlesim1',
        executable='tele_op',
        name='tele_op_node',
        # namespace=namespace,
        output="screen",
        
    # arguments=['--ros-args', '--log-level', 'debug'],
    #emulate_tty=True)
    )
    
    bt_navigator_node = Node(
        package='nav2_bt_navigator',
        # namespace='turtlesim1',
        executable='bt_navigator',
        name='bt_navigator_node',
        # namespace=namespace,
        output="screen",
        
    # arguments=['--ros-args', '--log-level', 'debug'],
    #emulate_tty=True)
    )    
    life_cycle_node = Node(
        package='nav2_lifecycle_manager',
        executable='lifecycle_manager',
        name='lifecycle_manager_localization',
        output='screen',
        parameters=[{'use_sim_time': False},
                    {'autostart': True},
                    {'node_names': ['bt_navigator_node']}])
    # Create the launch description and populate
    ld = LaunchDescription()

    # Declare the launch options
    ld.add_action(fbl_node)
    ld.add_action(fbs_node)
    # ld.add_action(bt_navigator_node)
    ld.add_action(TimerAction(period=5.0,actions=[tele_op_node]))
    # ld.add_action(TimerAction(period=6.0,actions=[life_cycle_node]))
    return ld

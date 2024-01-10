from launch import LaunchDescription
from launch_ros.actions import Node

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

    # Create the launch description and populate
    ld = LaunchDescription()

    # Declare the launch options
    ld.add_action(fbl_node)
    ld.add_action(fbs_node)
    ld.add_action(tele_op_node)

    return ld

import rclpy
from std_msgs.msg import Int32

topic="ros2_qt_demo_publish"

# Initialize ROS node
rclpy.init()
node = rclpy.create_node('sender')

publisher= node.create_publisher(Int32,topic, 10)

while True:
    i=input("Enter char to send: ")
    if i=='':break
    msg = Int32()
    msg.data = int(i) 
    print(msg)   
    publisher.publish(msg)


node.destroy_node()
rclpy.shutdown()
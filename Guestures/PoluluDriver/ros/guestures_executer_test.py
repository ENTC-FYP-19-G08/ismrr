import rclpy
from std_msgs.msg import Int8

topic="guesture"

# Initialize ROS node
rclpy.init()
node = rclpy.create_node('guestures_executer_test')

publisher= node.create_publisher(Int8,topic, 10)

while True:
    i=input("Enter char to send: ")
    if i=='':break
    msg = Int8()
    msg.data = int(i) 
    print(msg)   
    publisher.publish(msg)


node.destroy_node()
rclpy.shutdown()
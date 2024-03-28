import rclpy
from std_msgs.msg import Int32,String
from rclpy.node import Node


topic="ui_publisher"

def message_callback(msg):   
    print("Received message:", msg.data, type(msg.data))

rclpy.init()
node = rclpy.create_node("listener")

# Create a subscription to the topic with your message type
subscription = node.create_subscription(
    String,  
    topic,  
    message_callback,
    10  # QoS profile, you can adjust this value
)



print("listening..")
try:
    rclpy.spin(node)
except KeyboardInterrupt:
    pass

node.destroy_node()
rclpy.shutdown()
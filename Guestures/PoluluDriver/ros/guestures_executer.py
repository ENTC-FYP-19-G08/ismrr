import rclpy
from std_msgs.msg import Int8
from rclpy.node import Node
import serial

topic="guesture"

guesture_map=[
    b'\x00', #Ayubowan
    b'\x05', #Left
    b'\x06'  #Right
]

def message_callback(msg):
    ser.write(b'\xa7')
    ser.write(guesture_map[msg.data])
    print("Received message:", msg.data, type(msg.data))

rclpy.init()
node = rclpy.create_node("guestures_executer")

# Create a subscription to the topic with your message type
subscription = node.create_subscription(
    Int8,  
    topic,  
    message_callback,
    10  # QoS profile, you can adjust this value
)

ser = serial.Serial('/dev/ttyACM0', 9600)

print("listening..")
try:
    rclpy.spin(node)
except KeyboardInterrupt:
    pass

ser.close()
node.destroy_node()
rclpy.shutdown()
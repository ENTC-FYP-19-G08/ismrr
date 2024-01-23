import rclpy
from std_msgs.msg import String
from rclpy.node import Node
import firebase_admin
from firebase_admin import credentials
from firebase_admin import db


topic="sender_test_topic"

cred = credentials.Certificate('/media/jetson/SSD/ros2_ws/src/firebase_packages/firebase_listener/firebase_listener/robot-receptionist-19-firebase-adminsdk-d2d0o-c6356ff7ef.json')
# Initialize the app with a service account, granting admin privileges
firebase_admin.initialize_app(cred, {
    'databaseURL': "https://robot-receptionist-19-default-rtdb.asia-southeast1.firebasedatabase.app/"
})

def message_callback(msg):    
    ref.child("pos").set(msg.data)
    print("Received message:", msg.data)

rclpy.init()
node = rclpy.create_node("firebase_sender")

ref = db.reference('/ISMRR/app')

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

import rclpy
from std_msgs.msg import String
import firebase_admin
from firebase_admin import credentials
from firebase_admin import db

# Initialize ROS node
rclpy.init()
node = rclpy.create_node('firebase_listener')

# Initialize Firebase Admin SDK
# Fetch the service account key JSON file contents
cred = credentials.Certificate('/media/jetson/SSD/ros2_ws/src/firebase_packages/firebase_listener/firebase_listener/robot-receptionist-19-firebase-adminsdk-d2d0o-c6356ff7ef.json')
# Initialize the app with a service account, granting admin privileges
firebase_admin.initialize_app(cred, {
    'databaseURL': "https://robot-receptionist-19-default-rtdb.asia-southeast1.firebasedatabase.app/"
})

# Define a callback function for the database event
def database_callback(event):
    print("event.data:",event.data)
    if event.event_type == 'put':
        print("all:",str((event.event_type,event.path, event.data)))
        topic=event.path[1:]

        if topic == "reach":
            if not topic:
                return


            print("topic:",topic)  
                    
            msg = String()
            msg.data = str(topic+" "+event.data)       
            publisher.publish(msg)

        elif "move" in topic:
            msg = String()
            topic,data = topic.split('/')
            msg.data = str(topic+" "+data)       
            print(topic)
            publisher.publish(msg)

publisher= node.create_publisher(String,'tele_op_cmd', 10)

# Set up the Firebase Realtime Database event listener
# ref = db.reference('/ISMRR/cmd/move')
ref = db.reference('/ISMRR/robot')
ref.listen(database_callback)

rclpy.spin(node)
node.destroy_node()
rclpy.shutdown()

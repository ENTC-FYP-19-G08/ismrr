import rclpy
from std_msgs.msg import String

# topic="navigation_info"
# topic="/ui/guide_options"
# topic="/ui/guide_navigation_result"
# topic="/ui/username"
topic="/ui/change_state"



# Initialize ROS node
rclpy.init()
node = rclpy.create_node('sender')

publisher= node.create_publisher(String,topic, 10)

while True:
    i=input("Enter char to send "+topic+": ")
    if i=='':break
    msg = String()
    msg.data = i
    print(msg)   
    publisher.publish(msg)


node.destroy_node()
rclpy.shutdown()
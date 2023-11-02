import rclpy
from std_msgs.msg import String
from rclpy.node import Node

topic="sender_test_topic"
rclpy.init()
node = rclpy.create_node("sender_test")
publisher=node.create_publisher(String, topic, 10)

print("Presse enter to send:")
i=0
while not input():
    msg = String()
    msg.data = str(str(2*i)+","+str(i))       
    publisher.publish(msg)
    print(msg.data,end="")
    i+=10

node.destroy_node()
rclpy.shutdown()


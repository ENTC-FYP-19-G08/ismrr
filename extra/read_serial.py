import rclpy
from std_msgs.msg import String
import sys

import serial

ser = serial.Serial(port='/dev/ttyACM1',baudrate=9600)


def publish_serial():
    rclpy.init()

    node = rclpy.create_node('serial_publisher')
    publisher = node.create_publisher(String, '/serial_data', 10)
 
    try:
        while rclpy.ok():
            value= ser.readline()
            valueInString=str(value,'UTF-8')
            print(valueInString)
    except:
        pass

    rclpy.spin_once(node)    
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    publish_serial()


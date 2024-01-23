#!/usr/bin/env python3
import serial

import rclpy
from rclpy.node import Node
from std_msgs.msg import String


l_speed = 0
r_speed = 0 
state = 15

class ArduinoSerial(Node):
    def __init__(self):
        super().__init__(node_name='arduino_serial_node')
       
        self.ser = serial.Serial(port='/dev/arduino_serial',baudrate=115200, timeout=0.005)

        self.publisher =  self.create_publisher(String, '/wheel_encoders', 1)
        self.subscription = self.create_subscription(String,'/wheel_velocity',  self.vel_callback,1)

        self.timer = self.create_timer(0.01, self.timer_callback)

    def timer_callback(self):
        global l_speed,r_speed, state
        serial_str  = f'{l_speed},{r_speed}'  
        serial_str += "/n"
        
        # if serial_str!=prev:
        if state<10:
            x= serial_str.encode()
            self.ser.write(x)
        
            value= self.ser.readline()
            valueInString=str(value,'UTF-8')
            

            wheel_odom_msg = String()
            wheel_odom_msg.data = valueInString[:-2]
            # print(valueInString, wheel_odom_msg.data)
            # print(wheel_odom_msg.data)
            if  wheel_odom_msg.data:
                self.publisher.publish(wheel_odom_msg)

        if l_speed==0 and r_speed ==0:
            state += 1
        else:
            state = 0


    def vel_callback(self,msg):
        global l_speed,r_speed
        l_speed, r_speed = msg.data.split()     
        # print(msg.data)
   


def main():
    rclpy.init()
    node = ArduinoSerial()
    rclpy.spin(node)
    node.destroy_node()

    rclpy.shutdown()

if __name__ == '__main__':
    main()
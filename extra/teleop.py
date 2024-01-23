#!/usr/bin/env python3
import serial

import rclpy
from rclpy.node import Node


from geometry_msgs.msg import Twist
from std_msgs.msg import String
from geometry_msgs.msg import PoseStamped
from nav_msgs.msg import Odometry

from nav2_simple_commander.robot_navigator import BasicNavigator


l_speed = 0
r_speed = 0 
state = 15

class TeleOp(Node):
    def __init__(self):
        super().__init__(node_name='tele_op_node')
       
        self.ser = serial.Serial(port='/dev/arduino_serial',baudrate=115200, timeout=0.005)

        self.publisher =  self.create_publisher(String, '/wheel_encoders', 1)
        self.subscription = self.create_subscription(String,'/tele_op_cmd',  self.tele_op_callback,10  )

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
            print(valueInString, wheel_odom_msg.data)
            # print(wheel_odom_msg.data)
            if  wheel_odom_msg.data:
                self.publisher.publish(wheel_odom_msg)

        if l_speed==0 and r_speed ==0:
            state += 1
        else:
            state = 0


    def tele_op_callback(self,msg):
        global l_speed,r_speed
        print(msg.data)


        cmd = msg.data.split()

        
        if cmd[0] == 'move':  
            
            if cmd[1] == 'front_prs':
                l_speed = 0.4
                r_speed = 0.4 

            elif cmd[1] == 'back_prs':
                l_speed = -0.4
                r_speed = -0.4 

            elif cmd[1] == 'left_prs':
                l_speed = 0.0
                r_speed = 0.4 

            elif cmd[1] == 'right_prs':
                l_speed = 0.4
                r_speed = 0.0

            else:
                l_speed = 0.0
                r_speed = 0.0   


            serial_str = ""

            # if r_speed == 0.0:
            #     serial_str += "0"
            # elif r_speed == 0.4:
            #     serial_str += "50"                
            # elif r_speed == 0.6:
            #     serial_str += "100"     
            # elif r_speed == -0.4:
            #     serial_str += "-50"                
            # elif r_speed == -0.6:
            #     serial_str += "-100"    

            # if l_speed == 0.0:
            #     serial_str += ",0"
            # elif l_speed == 0.4:
            #     serial_str += ",50"                
            # elif l_speed == 0.6:
            #     serial_str += ",100"     
            # elif l_speed == -0.4:
            #     serial_str += ",-50"                
            # elif l_speed == -0.6:
            #     serial_str += ",-100"  

    


def main():
    rclpy.init()
    node = TeleOp()
    rclpy.spin(node)
    node.destroy_node()

    rclpy.shutdown()

if __name__ == '__main__':
    main()
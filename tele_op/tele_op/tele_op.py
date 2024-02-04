#!/usr/bin/env python3
import serial

import rclpy
from rclpy.node import Node


from geometry_msgs.msg import Twist
from std_msgs.msg import String


l_speed = 0
r_speed = 0 
state = 15

            
class TeleOp(Node):
    def __init__(self):
        super().__init__(node_name='tele_op_node')

        self.publisher =  self.create_publisher(Twist, '/cmd_vel_smoothen', 1)

        self.subscription = self.create_subscription(String,'/tele_op_cmd',  self.tele_op_callback,10  )

        self.timer = self.create_timer(0.01, self.timer_callback)

    def timer_callback(self):
        global l_speed,r_speed, state
        serial_str  = f'{l_speed},{r_speed}'  
        serial_str += "/n"
        
        # if serial_str!=prev:
        if state<10:

            vel_msg = Twist()
            vel_msg.linear.x = (l_speed + r_speed)/2
            vel_msg.angular.z = (r_speed - l_speed)/(0.465)
    
            # print(wheel_odom_msg.data)
            if  l_speed or r_speed or state <10:
                self.publisher.publish(vel_msg)

        if l_speed==0 and r_speed ==0:
            state += 1
        else:
            state = 0


    def tele_op_callback(self,msg):
        global l_speed,r_speed
        print(msg.data)

        cmd = msg.data.split(";")
        
        if cmd[0] == 'move': 

            act_dir = eval(cmd[1])
             
            if sum(1 for value in act_dir.values() if value)>2:
                l_speed = 0.0
                r_speed = 0.0  
                print("Invalid")
                return

            if act_dir["F"]:
                l_speed = 0.3
                r_speed = 0.3
                if act_dir["L"]:
                    l_speed = 0.1
                    r_speed = 0.4
                elif act_dir["R"]:
                    l_speed = 0.4
                    r_speed = 0.1     

            elif act_dir["B"]:
                l_speed = -0.3
                r_speed = -0.3
                if act_dir["L"]:
                    l_speed = -0.1
                    r_speed = -0.4
                elif act_dir["R"]:
                    l_speed = -0.4
                    r_speed = -0.1   

            elif act_dir["L"]:
                l_speed = -0.3
                r_speed = +0.3 

            elif act_dir["R"]:
                l_speed = +0.3
                r_speed = -0.3 
                
            else:
                l_speed = 0.0
                r_speed = 0.0   


    


def main():
    rclpy.init()
    node = TeleOp()
    rclpy.spin(node)
    node.destroy_node()

    rclpy.shutdown()

if __name__ == '__main__':
    main()
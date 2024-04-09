#!/usr/bin/env python3
import serial

import rclpy
from rclpy.node import Node

from nav2_simple_commander.robot_navigator import BasicNavigator
from geometry_msgs.msg import Twist
from std_msgs.msg import String
from geometry_msgs.msg import PoseStamped
from nav_msgs.msg import Odometry

from tf2_ros import TransformException
from tf2_ros.buffer import Buffer
from tf2_ros.transform_listener import TransformListener

l_speed = 0
r_speed = 0 
state = 15

            
class TeleOp(Node):
    def __init__(self):
        super().__init__(node_name='tele_op_node')

        self.publisher =  self.create_publisher(Twist, '/cmd_vel_raw', 1)
        self.goal_pub =  self.create_publisher(PoseStamped, '/app_goal', 4)
        self.pose_plisher = self.create_publisher(String,'/sender_test_topic', 10)
        
        self.subscription = self.create_subscription(String,'/tele_op_cmd',  self.tele_op_callback,10  )
        # self.subscription = self.create_subscription(Odometry,'/odometry/filtered',  self.odom_callback,10  )


        self.tf_buffer = Buffer()
        self.tf_listener = TransformListener(self.tf_buffer, self)
        # self.navigator = BasicNavigator()
        # self.navigator.waitUntilNav2Active()

        self.resolution = 0.05
        self.origin_x = -21.3
        self.origin_y = -20.1
        self.width = 607
        self.height = 653

        self.pose_x = None
        self.pose_y = None


        self.timer = self.create_timer(0.01, self.timer_callback)
        self.timer2 = self.create_timer(0.5, self.tf_callback)

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

        elif cmd[0] == 'reach':
            coords = list(map(int,cmd[1].split(",")))
            self.reach(coords[0],self.height - coords[1])
            
    def pixel_to_grid_cvt(self,coords):
        return (coords[0]*self.resolution + self.origin_x, coords[1]*self.resolution + self.origin_y)
    
    def grid_to_pixel_cvt(self,coords):
        return ((coords[0]- self.origin_x)//self.resolution , (coords[1]- self.origin_y)//self.resolution)
    
    def reach(self, pixel_loc_x, pixel_loc_y):    
        grid_loc = self.pixel_to_grid_cvt([pixel_loc_x,pixel_loc_y])
        goal_pose = PoseStamped()
        goal_pose.header.frame_id = 'map'
        goal_pose.header.stamp = self.get_clock().now().to_msg()
        goal_pose.pose.position.x = grid_loc[0]
        goal_pose.pose.position.y = grid_loc[1]
        self.goal_pub.publish(goal_pose)
    
    def tf_callback(self):
        from_frame_rel = "base_link"
        to_frame_rel = 'map'

        try:
            t = self.tf_buffer.lookup_transform(
                to_frame_rel,
                from_frame_rel,
                rclpy.time.Time())
        except TransformException as ex:
            self.get_logger().info(f'Could not transform {to_frame_rel} to {from_frame_rel}: {ex}')
            return


        self.pose_x =  t.transform.translation.x
        self.pose_y =  t.transform.translation.y


        if self.pose_x == None or self.pose_y == None:
            return
        
        x,y = self.grid_to_pixel_cvt([self.pose_x,self.pose_y] )
        msg_pose = String()
        msg_pose.data = str(x)+","+str(self.height - y)
        self.pose_plisher.publish(msg_pose)

def main():
    rclpy.init()
    node = TeleOp()
    rclpy.spin(node)
    node.destroy_node()

    rclpy.shutdown()

if __name__ == '__main__':
    main()
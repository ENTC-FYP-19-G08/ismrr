#!/usr/bin/env python3
import rclpy
from rclpy.node import Node


from geometry_msgs.msg import Twist
from std_msgs.msg import String
from geometry_msgs.msg import PoseStamped

from nav2_simple_commander.robot_navigator import BasicNavigator

class TeleOp(Node):
    def __init__(self):
        super().__init__(node_name='tele_op_node')
        
        ############## Subscribers ##############
        self.subscription = self.create_subscription(String,'/tele_op_cmd',  self.tele_op_callback,10  )
 
        ############## Publishers ##############
        self.velocity_publisher = self.create_publisher(Twist,'/cmd_vel_raw', 10)


        initial_pose = PoseStamped()
        initial_pose.header.frame_id = 'map'    
        initial_pose.header.stamp = self.get_clock().now().to_msg()
        initial_pose.pose.position.x = -2.0
        initial_pose.pose.position.y =0 -0.5

        self.navigator = BasicNavigator()
        self.navigator.setInitialPose(initial_pose)
        self.navigator.waitUntilNav2Active()

        self.resolution = 0.05
        self.origin_x = -10
        self.origin_y = -10
        self.reach([197,197])
        

    def pixel_to_grid_cvt(self,coords):
        return (coords[0]*self.resolution + self.origin_x, coords[1]*self.resolution + self.origin_y)

    def reach(self, pixel_loc):
        grid_loc = self.pixel_to_grid_cvt(pixel_loc)
        goal_pose = PoseStamped()
        goal_pose.header.frame_id = 'map'
        goal_pose.header.stamp = self.navigator.get_clock().now().to_msg()
        goal_pose.pose.position.x = grid_loc[0]
        goal_pose.pose.position.y = grid_loc[1]
        self.navigator.goToPose(goal_pose)

    def tele_op_callback(self,msg):
        print(msg.data)
        vel = Twist()

        if msg.data == 'front_prs':
            vel.linear.x = 0.4
        elif msg.data == 'front_rls':
            vel.linear.x = 0.0  
        elif msg.data == 'back_prs':
            vel.linear.x = -0.4
        elif msg.data == 'back_rls':
            vel.linear.x = 0.0  
        elif msg.data == 'left_prs':
            vel.angular.z = 0.4
        elif msg.data == 'left_rls':
            vel.angular.z = 0.0  
        elif msg.data == 'right_prs':
            vel.angular.z = -0.4
        elif msg.data == 'right_rls':
            vel.angular.z = 0.0     

        self.velocity_publisher.publish(vel)
    
    


def main():
    rclpy.init()
    node = TeleOp()
    rclpy.spin(node)
    node.destroy_node()

    rclpy.shutdown()

if __name__ == '__main__':
    main()
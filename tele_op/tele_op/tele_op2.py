#!/usr/bin/env python3
import rclpy
from rclpy.node import Node


from geometry_msgs.msg import Twist
from std_msgs.msg import String
from geometry_msgs.msg import PoseStamped
from nav_msgs.msg import Odometry

from nav2_simple_commander.robot_navigator import BasicNavigator

class TeleOp(Node):
    def __init__(self):
        super().__init__(node_name='tele_op_node')
        
        ############## Subscribers ##############
        self.subscription = self.create_subscription(String,'/tele_op_cmd',  self.tele_op_callback,10  )
        self.subscription = self.create_subscription(Odometry,'/odom',  self.odom_callback,10  )

        ############## Publishers ##############
        self.velocity_publisher = self.create_publisher(Twist,'/cmd_vel_raw', 10)
        self.pose_plisher = self.create_publisher(String,'/sender_test_topic', 10)


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
        self.width = 384
        self.height = 384

        self.pose_x = None
        self.pose_y = None

    def pixel_to_grid_cvt(self,coords):
        return (coords[0]*self.resolution + self.origin_x, coords[1]*self.resolution + self.origin_y)
    
    def grid_to_pixel_cvt(self,coords):
        return ((coords[0]- self.origin_x)//self.resolution , (coords[1]- self.origin_y)//self.resolution)
    
    def reach(self, pixel_loc_x, pixel_loc_y):
        grid_loc = self.pixel_to_grid_cvt([pixel_loc_x,pixel_loc_y])
        goal_pose = PoseStamped()
        goal_pose.header.frame_id = 'map'
        goal_pose.header.stamp = self.navigator.get_clock().now().to_msg()
        goal_pose.pose.position.x = grid_loc[0]
        goal_pose.pose.position.y = grid_loc[1]
        self.navigator.goToPose(goal_pose)

  
    def odom_callback(self,msg):
        pos = msg
        self.pose_x = pos.pose.pose.position.x
        self.pose_y = pos.pose.pose.position.y


        if self.pose_x == None or self.pose_y == None:
            return
        
        x,y = self.grid_to_pixel_cvt([self.pose_x,self.pose_y] )
        msg_pose = String()
        msg_pose.data = str(x)+","+str(self.height - y)
        self.pose_plisher.publish(msg_pose)


    def tele_op_callback(self,msg):
        print(msg.data)
        vel = Twist()

        cmd = msg.data.split()
        if cmd[0] ==  'reach':
            coords = list(map(int,cmd[1].split(",")))
            self.reach(coords[0],self.height - coords[1])

        elif cmd[0] == 'move':    
            if cmd[1] == 'front_prs':
                vel.linear.x = 0.4
            elif cmd[1] == 'front_rls':
                vel.linear.x = 0.0  
            elif cmd[1] == 'back_prs':
                vel.linear.x = -0.4
            elif cmd[1] == 'back_rls':
                vel.linear.x = 0.0  
            elif cmd[1] == 'left_prs':
                vel.angular.z = 0.4
            elif cmd[1] == 'left_rls':
                vel.angular.z = 0.0  
            elif cmd[1] == 'right_prs':
                vel.angular.z = -0.4
            elif cmd[1] == 'right_rls':
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

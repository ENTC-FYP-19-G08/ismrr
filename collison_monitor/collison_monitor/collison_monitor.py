#!/usr/bin/env python3
import rclpy
from rclpy.node import Node

from sensor_msgs.msg import LaserScan
from geometry_msgs.msg import Twist
from geometry_msgs.msg import PolygonStamped, Point32
from nav_msgs.msg import Odometry


import math

class CollisonMonitor(Node):
    def __init__(self):
        super().__init__(node_name='collison_monitor_node')
        
        ############## Subscribers ##############
        self.subscription = self.create_subscription(Odometry,'/odom',  self.odom_callback,10  )
        self.subscription = self.create_subscription(LaserScan,'/scan',  self.laser_callback,10  )
        self.raw_velocity_subscription = self.create_subscription(Twist,'/cmd_vel_raw',  self.raw_velocity_callback,10)
        
        ############## Publishers ##############
        self.velocity_publisher = self.create_publisher(Twist,'/cmd_vel', 10)
        self.slow_down_polygon_publisher = self.create_publisher(PolygonStamped,'/collison_slow_down_polygon', 10)
        self.stop_polygon_publisher = self.create_publisher(PolygonStamped,'/collison_stop_polygon', 10)
     

        ################################# Set these to handle collisions ########################
        self.slow_down_radius = 0.4
        self.stop_radius = 0.28
        self.reducing_factor = 0.5 

        self.warning_level = 0 #0-no obstacle 1-slow_down 2-stop

        self.collison_polygon = PolygonStamped()
        self.collison_polygon.header.frame_id = 'map'
        self.pose_x = 0
        self.pose_y = 0

        self.slow_down_zero_origin_points = []
        self.stop_zero_origin_points = []

        for i in range(0,360):
            theta = i*math.pi/180
            self.point = Point32()
            self.point.x = self.slow_down_radius*math.cos(theta)
            self.point.y = self.slow_down_radius*math.sin(theta)
            self.slow_down_zero_origin_points.append(self.point)

            self.point = Point32()
            self.point.x = self.stop_radius*math.cos(theta)
            self.point.y = self.stop_radius*math.sin(theta)
            self.stop_zero_origin_points.append(self.point)

        self.polygon_thread = self.create_timer(0.5,self.publish_polygons)

    def publish_polygons(self):
        offset_x = self.pose_x + 0.03
        offset_y = self.pose_y + 0.03
        self.collison_polygon.polygon.points.clear()

        for point in self.slow_down_zero_origin_points:
            shifted_point = Point32()
            shifted_point.x = offset_x + point.x
            shifted_point.y = offset_y +  point.y
            self.collison_polygon.polygon.points.append(shifted_point)

        self.collison_polygon.header.stamp = self.get_clock().now().to_msg()
        self.slow_down_polygon_publisher.publish(self.collison_polygon)

        self.collison_polygon.polygon.points.clear()
        for point in self.stop_zero_origin_points:
            shifted_point = Point32()
            shifted_point.x = offset_x + point.x
            shifted_point.y = offset_y +  point.y
            self.collison_polygon.polygon.points.append(shifted_point)            

        self.collison_polygon.header.stamp = self.get_clock().now().to_msg()
        self.stop_polygon_publisher.publish(self.collison_polygon)
        


    def odom_callback(self,msg):
        pos = msg
        self.pose_x = pos.pose.pose.position.x
        self.pose_y = pos.pose.pose.position.y
    
        
      
    def raw_velocity_callback(self, msg):

        if self.warning_level == 0:
            self.get_logger().info("Follow Path")  
            self.velocity_publisher.publish(msg)

        elif self.warning_level == 2:
            self.get_logger().info("Stopped")  
            vel = Twist()
            self.velocity_publisher.publish(vel)

        else:
            self.get_logger().info("Slowing Down")  
            vel = Twist()
            vel.linear.x = msg.linear.x * self.reducing_factor
            vel.linear.y = msg.linear.y * self.reducing_factor
            vel.linear.z = msg.linear.z * self.reducing_factor
            vel.angular.x = msg.angular.x * self.reducing_factor
            vel.angular.y = msg.angular.y * self.reducing_factor
            vel.angular.z = msg.angular.z * self.reducing_factor        
            self.velocity_publisher.publish(vel)


    def laser_callback(self, msg):

        min_distance_to_obstacle = min(msg.ranges)
        if min_distance_to_obstacle < self.stop_radius:
            self.warning_level = 2
        elif min_distance_to_obstacle < self.slow_down_radius:
            self.warning_level = 1

        else:
            self.warning_level = 0  


def main():
    rclpy.init()
    node = CollisonMonitor()
    rclpy.spin(node)
    node.destroy_node()

    rclpy.shutdown()

if __name__ == '__main__':
    main()
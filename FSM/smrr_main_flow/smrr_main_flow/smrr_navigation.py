from enum import Enum

from geometry_msgs.msg import PoseStamped
from rclpy.duration import Duration


import rclpy
from rclpy.action import ActionClient
from rclpy.node import Node
from rclpy.qos import QoSDurabilityPolicy, QoSHistoryPolicy
from rclpy.qos import QoSProfile, QoSReliabilityPolicy

from nav2_msgs.action import  NavigateToPose
from action_msgs.msg import GoalStatus
from std_msgs.msg import String, Int8

# from .load_locations import LoadLocations
# from .robot_navigator import RobotNavigator,TaskResult


from load_locations import LoadLocations
from robot_navigator import RobotNavigator,TaskResult

import time 

from rclpy.executors import SingleThreadedExecutor,MultiThreadedExecutor
import threading

class SMRRNavigation():
    def __init__(self, node):
        self.node = node
        self.locations = LoadLocations(self.node).locations
        self.goal = None
        self.ui_sub = self.node.create_subscription(String, '/ui_publisher', self.ui_callback, 1)
        self.timer = None 
        self.navi_obj = RobotNavigator()
        self.nav_state = self.node.create_publisher(String, '/nav_goal_state', 1)

    def execute_thread(self):
        self.navi_obj.busy  = True
        self.timer.cancel()
        self.navi_obj.reach_destination(self.goal)

        while(self.navi_obj.finish != True):
            print(self.navi_obj.finish)
            pass

        print(self.navi_obj.getResult())
        if self.navi_obj.getResult() == TaskResult.SUCCEEDED:
                print("Done")
        else:
            print("Falied to reach")
        
        self.navi_obj.finish = False 
        self.navi_obj.busy = False   

        msg = Int8()
        msg.data =  self.navi_obj.getResult()
        self.nav_state.publish(msg)

    def ui_callback(self, msg):
        if self.navi_obj.busy:
            self.node.get_logger().info("Robot is busy. Please wait until the task is completed")
            return
        
        self.navi_obj.cancel_navigation_goals()
        self.busy = True
        data = msg.data.split("/")
        if data[0] != "navigation":
            return
       
        self.node.get_logger().info("Navigating to " + data[1])
        self.goal = self.locations[data[1]]
        self.timer = self.node.create_timer(1, self.execute_thread)



   
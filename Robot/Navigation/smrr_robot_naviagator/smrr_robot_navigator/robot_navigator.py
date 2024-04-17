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
import rclpy.time
from std_msgs.msg import String, Bool, Empty, Int8
from rclpy.executors import MultiThreadedExecutor


from tf2_ros import TransformException
from tf2_ros.buffer import Buffer
from tf2_ros.transform_listener import TransformListener

import math 

##################### This Package was wriitten due to unavailability of simple commander package for ros2 foxy #####################

GOAL_TOLERANCE = 0.4 #The radius around the goal, can be considered as a successful navigation

class TaskResult():
    UNKNOWN = 0
    SUCCEEDED = 1
    CANCELED = 2
    FAILED = 3

class RobotNavigator(Node):
    def __init__(self):
        super().__init__('navigator')

        self.goal = None #Navigation goal
        self.status = None #Navigation status
        self.busy = False #Check whether already has a goal
        self.current_pose = None #current pose

        self.tf_buffer = Buffer()
        self.tf_listener = TransformListener(self.tf_buffer, self)

        self.nav_to_pose_client = ActionClient(self, NavigateToPose, 'navigate_to_pose')

        self.nav_result_pub = self.create_publisher(Int8, '/nav_result', 10)
        self.result = Int8()

        self.client = self.create_subscription(Empty, '/emergency_stop', self.emergency_callback, 10)
        self.goal_client = self.create_subscription(String, 'nav_goal', self.reach_destination, 10)

        self.goal_timer = self.create_timer(2,self.execute_goal)
        self.tf_timer = self.create_timer(1,self.tf_listen)

    def tf_listen(self):
        print(self.current_pose)
        # if not self.busy:
        #     return

        try:
            t = self.tf_buffer.lookup_transform(
                "map",
                "base_link",
                rclpy.time.Time())
            
            if self.current_pose == None:
                self.current_pose = dict()
            self.current_pose['x'] = t.transform.translation.x
            self.current_pose['y'] = t.transform.translation.y
        
            if (self.goal != None) and (math.hypot(self.goal.pose.pose.position.x - self.current_pose['x'] , self.goal.pose.pose.position.y - self.current_pose['y'])<GOAL_TOLERANCE):
                self.cancelTask()
                self.goal = None
                self.busy = False
                self.result.data = TaskResult.SUCCEEDED
                self.nav_result_pub.publish(self.result)

        except TransformException as ex:
            return

    def execute_goal(self):
        if self.goal == None:
            return
        
        self.busy = True
        self.nav_to_pose_client.send_goal_async(self.goal)  
             
        

    def emergency_callback(self,msg):
        self.cancelTask()
        self.goal = None
        self.busy = False
        self.result.data = TaskResult.CANCELED
        self.nav_result_pub.publish(self.result)

    def reach_destination(self,msg):
        pose = eval(msg.data)
        goal_pose = PoseStamped()
        goal_pose.header.frame_id = 'map'
        goal_pose.header.stamp = self.get_clock().now().to_msg()
        goal_pose.pose.position.x = pose[0]
        goal_pose.pose.position.y = pose[1]
        goal_pose.pose.orientation.w = 1.0
       
        self.goal = NavigateToPose.Goal()
        self.goal.pose = goal_pose

        while not self.nav_to_pose_client.wait_for_server(timeout_sec=1.0):
            self.info("'NavigateToPose' action server not available, waiting...")
        
    
    def cancelTask(self):
        """Cancel pending task request of any type."""
        self.info('Canceling current task.')
        goal_pose = PoseStamped()
        goal_pose.header.frame_id = 'map'
        goal_pose.header.stamp = self.get_clock().now().to_msg()
        goal_pose.pose.position.x = self.current_pose['x']
        goal_pose.pose.position.y = self.current_pose['y']
        goal_pose.pose.orientation.w = 1.0

        temp = NavigateToPose.Goal()
        temp.pose = goal_pose
        self.nav_to_pose_client.send_goal_async(temp)
    
    def info(self, msg):
        self.get_logger().info(msg)
        return

    def warn(self, msg):
        self.get_logger().warn(msg)
        return

    def error(self, msg):
        self.get_logger().error(msg)
        return

    def debug(self, msg):
        self.get_logger().debug(msg)
        return
    

def main(args=None):
    rclpy.init(args=args)

    rn = RobotNavigator()

    rclpy.spin(rn)

    rn.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
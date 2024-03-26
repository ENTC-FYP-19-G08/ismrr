import rclpy
from rclpy.node import Node
from std_msgs.msg import String

from .smrr_navigation import SMRRNavigation
from .smrr_navigation import TaskResult
from .smrr_gestures import SMRRGestures
from .smrr_gestures import GestureType
from .smrr_face_recognition import SMRRFaceRecogition
from .load_locations import LoadLocations

from visualization_msgs.msg import MarkerArray
from visualization_msgs.msg import Marker

from rclpy.qos import QoSProfile
from rclpy.qos import ReliabilityPolicy, DurabilityPolicy, HistoryPolicy


class MainFlow(Node):
    def __init__(self):
        super().__init__('flow_node')

        _ = LoadLocations(self)
        nav_obj = SMRRNavigation(self)
        # gesture_obj = SMRRGestures(self)
        # face_recsog_obj = SMRRFaceRecogition(self)

   


        # navigation_result = nav_obj.reach_destination(self.locations["ENTC1"])
        # if navigation_result == TaskResult.SUCCEEDED:
        #         print("Done")
 

        # navigation_result = nav_obj.reach_destination(self.locations["Entc1"])
        # if navigation_result == TaskResult.SUCCEEDED:
        #     print('Done')
        # print(face_recog_obj.send_request())


        
        # self.publisher_ = self.create_publisher(String, 'topic', 10)
        # timer_period = 0.5  # seconds
        # self.timer = self.create_timer(timer_period, self.timer_callback)
        # self.i = 0
  
def main(args=None):
    rclpy.init(args=args)

    main_flow = MainFlow()

    rclpy.spin(main_flow)

    main_flow.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
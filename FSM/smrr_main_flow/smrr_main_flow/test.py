import rclpy
from rclpy.node import Node
from std_msgs.msg import String

from .smrr_navigation import SMRRNavigation
# from .smrr_navigation import TaskResult
from .smrr_gestures import SMRRGestures
from .smrr_gestures import GestureType
# from .smrr_face_recognition import SMRRFaceRecogition
from .smrr_conversation import SMRRCoversation
        
class Test(Node):
    def __init__(self):
        super().__init__('flow_node')

        # _ = LoadLocations(self)
        # nav_obj = SMRRNavigation(self)
        self.conv_obj = SMRRCoversation()
        
        # gesture_obj = SMRRGestures(self)
        # face_recsog_obj = SMRRFaceRecogition(self)
        # gesture_obj.do_gesture(GestureType.SHOW_LEFT

 
        # navigation_result = nav_obj.reach_destination(self.locations["ENTC1"])
        # if navigation_result == TaskResult.SUCCEEDED:
        #         print("Done")
 

        # navigation_result = nav_obj.reach_destination(self.locations["Entc1"])
        # if navigation_result == TaskResult.SUCCEEDED:
        #     print('Done')
        # print(face_recog_obj.send_request())


        
        # self.publisher_ = self.create_publisher(String, 'topic', 10)
        # timer_period = 0.5  # seconds
        self.timer = self.create_timer(1, self.timer_callback)
        # self.i = 0
    def timer_callback(self):
        self.timer.cancel()
        self.conv_obj.start_listening()

def main(args=None):
    rclpy.init(args=args)

    test = Test()

    rclpy.spin(test)

    test.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
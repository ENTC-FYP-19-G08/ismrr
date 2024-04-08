import rclpy
from rclpy.node import Node
from std_msgs.msg import String

# from .smrr_navigation import SMRRNavigation
# from .smrr_navigation import TaskResult
# from .smrr_gestures import SMRRGestures
# from .smrr_gestures import GestureType
# from .smrr_face_recognition import SMRRFaceRecogition
# from .load_locations import LoadLocations
# from .smrr_conversation import SMRRCoversation
# from .messages import waiting_messages, thanking_messages, welcoming_messages

# from smrr_navigation import SMRRNavigation
# from smrr_navigation import TaskResult
from smrr_gestures import SMRRGestures
from smrr_gestures import GestureType
from smrr_face_recognition import SMRRFaceRecogition
from load_locations import LoadLocations
from smrr_conversation import SMRRCoversation
from face_recog_interfaces.srv import FaceRecogRequest
from messages import waiting_messages, thanking_messages, welcoming_messages


from simple_node import Node
from yasmin import State
from yasmin import StateMachine
from yasmin_viewer import YasminViewerPub
import time
import random
from yasmin_ros.basic_outcomes import SUCCEED, ABORT, CANCEL

class LoadModules(State):
    def __init__(self, node):
        super().__init__([SUCCEED, ABORT])
        self.node = node

    def execute(self, blackboard):

        print("Executing state Load Modules")
        blackboard.conv_obj = SMRRCoversation(self.node)
        print("Loading conversation module successful")
        # blackboard.face_recog_obj = SMRRFaceRecogition(self)
        print("Loading face recognition module successful")
        # blackboard.nav_obj = SMRRNavigation(self)
        print("Loading navigation module successful")
        # blackboard.gestures_obj = SMRRGestures(self)
        print("Loading gestures module successful")
        time.sleep(1)
        return SUCCEED
       

class Idle(State):
    def __init__(self, node):
        super().__init__(["trigger","end"])
        self.node = node
        # self.name_pub = self.node.create_publisher(String, '/trigger', 10)
        self.trigger = False
        # self.conv_obj = SMRRCoversation(self)
        # print("Conversation module is ready")
    
    def call_back(self, msg):
        self.trigger = True

    def execute(self, blackboard):
        print("Executing state Idle")
        # try:
        #     print(blackboard.conv_obj)
        # except:
        #     blackboard.conv_obj = self.conv_obj

      
        # blackboard.conv_obj.wait_idle()
      
        return "trigger"

# define state Conversation
class Conversation(State):
    def __init__(self, node):
        super().__init__(["guide","end"])
        self.node = node
        # gesture_obj = SMRRGestures(self.node)
        # print("Gestures module is ready")
        # face_recsog_obj = SMRRFaceRecogition(self.node)
        # print("Face recognition module is ready")
        self.fr_cli = self.node.create_client(FaceRecogRequest, '/smrr_face_recog_srv')
        self.name_pub = self.node.create_publisher(String, '/ui/username', 10)   

        # while not self.fr_cli.wait_for_service(timeout_sec=1.0):
        #     self.node.get_logger().info('face recognition service not available, waiting again...')
        # self.node.get_logger().info("Face recognition service  available")
        # self.req = FaceRecogRequest.Request()

    def execute(self, blackboard):
        print("Executing state Conversation")
        blackboard.conv_obj.text_to_speech(random.choice(welcoming_messages))
        blackboard.conv_obj.text_to_speech(random.choice(waiting_messages))
        # name, angle = self.trigger_func()
        # print(name, angle)
        # msg = String()
        # msg.data = name
        # self.name_pub.publish(msg)
        # if name!= 'unknown':
        #     blackboard.conv_obj.text_to_speech("Hi"+name + "Nice to see you again.")
        # else:
        #     blackboard.conv_obj.text_to_speech("We haven't met before. Could i know your name please? If you dont mind. Or you can skip.")
        # self.conv_obj.start_listening()
        blackboard.conv_obj.start_listening()
        return "guide"
    
    def trigger_func(self):
        self.req.name_request = True
        self.req.angle_request = True
        self.future = self.fr_cli.call_async(self.req)
        print("sent")
        rclpy.spin_until_future_complete(self.node, self.future)
        print("recieved")
        response = self.future.result()
        return [response.name,response.angle]
# define state Navigate
class Navigation(State):
    def __init__(self):
        super().__init__([SUCCEED, ABORT, CANCEL])
        # nav_obj = SMRRNavigation(self.node)
        # print("Navigation module is ready")

    def execute(self, blackboard):
        print("Executing state Navigation")
        time.sleep(1)
        return SUCCEED
    
class Guide(State):
    def __init__(self):
        super().__init__(["navigate","gestures"])

    def execute(self, blackboard):
        print("Executing state Guide")      
        time.sleep(1)
        return "navigate"

# define state BackHome
class BackHome(State):
    def __init__(self):
        super().__init__([SUCCEED])
      
    def execute(self, blackboard):
        print("Executing state BackHome")
        time.sleep(1)
        blackboard.nav = False
        return SUCCEED

# define state SwitchingPowerMode
class SwitchingPowerMode(State):
    def __init__(self):
        super().__init__(['navigate', 'idle'])
    def execute(self, blackboard):
        print("Executing state SwitchingPowerMode")
        time.sleep(1)
        if blackboard.nav:
            return 'navigate'
        else:
            return 'idle'
        
class MainFlow(Node):
    def __init__(self):
        super().__init__('flow_node')
    
        _ = LoadLocations(self)

        # create a state machine
        sm = StateMachine(outcomes=["END"])

        sm.add_state("LOADMODULES",LoadModules(self),
                     transitions={SUCCEED: "IDLE",
                                  ABORT: "LOADMODULES"})

        sm.add_state("IDLE",Idle(self),
                     transitions={"trigger": "COVERSATION",
                                  "end":"END"})

        sm.add_state("GUIDE", Guide(),
                     transitions={"navigate": "SWITCHINGPOWERMODE",
                                  "gestures":"IDLE"})
        
        sm.add_state("COVERSATION", Conversation(self),
                     transitions={"end": "IDLE",
                                  "guide": "GUIDE"}
                                  )
        
        sm.add_state("SWITCHINGPOWERMODE", SwitchingPowerMode(),
                    transitions={'navigate': "NAVIGATION",
                                 'idle': "IDLE"
                                 }                     
                    )

        sm.add_state("NAVIGATION", Navigation(),
                     transitions={SUCCEED: "BACKHOME",
                                  ABORT:"NAVIGATION",
                                  CANCEL:"BACKHOME"})    

        sm.add_state("BACKHOME", BackHome(),
                     transitions={SUCCEED: "SWITCHINGPOWERMODE",
                                  ABORT:"BACKHOME",
                                  CANCEL:"END"})   
        
        YasminViewerPub(self, "SMRR_FSM", sm)
    
        # execute
        outcome = sm()
        print(outcome)
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

    rclpy.join_spin(main_flow)

    main_flow.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
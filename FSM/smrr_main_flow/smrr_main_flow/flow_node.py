import rclpy
from rclpy.node import Node
from std_msgs.msg import String
from std_msgs.msg import String, Int8
# from .smrr_navigation import SMRRNavigation
# from .smrr_navigation import TaskResult
# from .smrr_gestures import SMRRGestures
# from .smrr_gestures import GestureType
# from .smrr_face_recognition import SMRRFaceRecogition
# from .load_locations import LoadLocations
# from .smrr_conversation import SMRRCoversation
# from .messages import waiting_messages, thanking_messages, welcoming_messages


# from smrr_navigation import TaskResult
# from smrr_gestures import SMRRGestures
# from smrr_gestures import GestureType
# from smrr_face_recognition import SMRRFaceRecogition
from load_locations import LoadLocations
from smrr_conversation import SMRRCoversation
# from face_recog_interfaces.srv import FaceRecogRequest
from messages import waiting_messages, thanking_messages, welcoming_messages
from smrr_navigation_results import TaskResult
from geometry_msgs.msg import PoseStamped
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

        # print("Executing state Load Modules")
        blackboard.conv_obj = SMRRCoversation(self.node)
        print("Loading conversation module successful")
        # blackboard.face_recog_obj = SMRRFaceRecogition(self.node)
        # print("Loading face recognition module successful")
        # blackboard.nav_obj = SMRRNavigation(self)
        # print("Loading navigation module successful")
        # blackboard.gestures_obj = SMRRGestures(self)
        # print("Loading gestures module successful")
        time.sleep(1)
        return SUCCEED
       
class Idle(State):
    def __init__(self, node):
        super().__init__(["trigger","end"])
        self.node = node
        # self.trig_sub = self.node.create_subscription(String,'/trigger', self.call_back,10)
        # self.trigger = False
        # print("Conversation module is ready")
    
    def call_back(self, msg):
        self.trigger = True

    def execute(self, blackboard):
        # self.trigger = False
        print("Executing Idle state ")
        # while not self.trigger:
        #     pass
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
        # self.fr_cli = self.node.create_client(FaceRecogRequest, '/smrr_face_recog_srv')
        
        # self.name_pub = self.node.create_publisher(String, '/ui/username', 10)   

        # while not self.fr_cli.wait_for_service(timeout_sec=1.0):
        #     self.node.get_logger().info('face recognition service not available, waiting again...')
        # self.node.get_logger().info("Face recognition service  available")
        # self.req = FaceRecogRequest.Request()

    def call_back(self,msg):
        self.trigger = True

    def execute(self, blackboard):
        print("Executing Conversation state")
        # blackboard.conv_obj.text_to_speech(random.choice(welcoming_messages))

        # SMRRGestures.do_gesture(GestureType.AYUBOWAN)
        # blackboard.conv_obj.text_to_speech("Ayubowan")
        
        # # blackboard.conv_obj.text_to_speech(random.choice(waiting_messages))
        # name, angle = self.trigger_func()
        # print(name, angle)
        # msg = String()
        # msg.data = name
        # self.name_pub.publish(msg)
        # if name!= 'unknown':
        #     blackboard.conv_obj.text_to_speech("Hi"+name + "Nice to see you again.")
        # else:
        #     blackboard.conv_obj.text_to_speech("We haven't met before. Could i know your name please? If you dont mind. Or you can skip.")
        
        
        blackboard.conv_obj.start_listening()
        print("Exite from conversation state")

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
    def __init__(self, node):
        super().__init__([SUCCEED, ABORT, CANCEL])
        self.node = node
        self.locations = LoadLocations(self.node).locations
        self.goal = None
        self.nav_result = None
        self.ui_sub = self.node.create_subscription(String, '/ui/guide_navigation', self.ui_callback, 10)
        self.app_goal_sub = self.node.create_subscription(PoseStamped, '/app_goal', self.app_goal_callback, 10)
        self.nav_result_sub = self.node.create_subscription(Int8, '/nav_result', self.nav_result_callback, 10)

        self.nav_state_pub = self.node.create_publisher(String, '/ui/guide_navigation_result', 10)
        self.nav_goal_pub = self.node.create_publisher(String, 'nav_goal', 10)
        self.nav_result_outcomes ={0: "UNKNOWN", 1:"SUCCEEDED", 2:"CANCELED" ,3:"FAILED"}
        self.timer = None

    def nav_result_callback(self, msg):
        self.nav_result = msg.data

    def app_goal_callback(self, msg):
        print("Destination from APP Recieved")        
        if self.goal:
            print("Navigator is busy. Try again later")
            return
        self.goal = self.locations[msg.data]

    def ui_callback(self, msg):
        print("Detination from UI Recieved")        
        if self.goal:
            print("Navigator is busy. Try again later")
            return
        self.goal = self.locations[msg.data]

    def go_back_home(self):
        self.goal = self.locations["HOME"]

    def publish_goal(self):
        msg = String()
        msg.data = str(self.goal)
        self.nav_goal_pub.publish(msg)

    def execute(self, blackboard):
        print("Executing state Navigation")\
        
        while True:
            while self.goal == None:
                pass

            self.publish_goal()        

            while self.nav_result == None:
                pass
        
            msg = String()
            msg.data = self.nav_result_outcomes[self.nav_result]
            self.nav_state_pub.publish(msg)
            self.nav_result = None
            
            if self.goal != self.locations["HOME"]:
                self.go_back_home()
            else:
                self.goal = None
                print("EXIT SUCCESSFUL FROM NAVIGATION")
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

        return SUCCEED

# define state SwitchingPowerMode
class SwitchingPowerMode(State):
    def __init__(self):
        super().__init__(['navigate', 'idle'])
    def execute(self, blackboard):
        print("Executing state SwitchingPowerMode")
        time.sleep(1)

        return 'navigate'

     
        
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

        sm.add_state("NAVIGATION", Navigation(self),
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
        
def main(args=None):
    rclpy.init(args=args)

    main_flow = MainFlow()

    rclpy.join_spin(main_flow)

    main_flow.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
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

from smrr_gestures import SMRRGestures,GestureType
from load_locations import LoadLocations
from smrr_conversation import SMRRCoversation
from face_recog_interfaces.srv import FaceRecogRequest
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
        blackboard.state_publisher = self.node.create_publisher(String, '/ui/change_state', 10)
        print("Loading conversation module successful")
        # blackboard.nav_obj = SMRRNavigation(self)
        # print("Loading navigation module successful")
        blackboard.gestures_obj = SMRRGestures()
        print("Loading gestures module successful")
        time.sleep(1)
        return SUCCEED
       
class Idle(State):
    def __init__(self, node):
        super().__init__(["trigger","end"])
        self.node = node
        self.trig_sub = self.node.create_subscription(String,'/trigger', self.call_back,10)
        self.trigger = False
        # print("Conversation module is ready")
    
    def call_back(self, msg):
        self.trigger = True

    def execute(self, blackboard):
        msg = String()
        msg.data = "IDLE"
        blackboard.state_publisher.publish(msg)
        self.trigger = False
        print("Executing Idle state ")
        while not self.trigger:
            time.sleep(1)
            pass
        self.trigger = False
        return "trigger"

# define state Conversation
class Conversation(State):
    def __init__(self, node):
        super().__init__(["guide","end"])
        self.node = node
        self.stop_listening_sub = self.node.create_subscription(String, '/ui/guide_navigation', self.stop_listening_callback, 10)
        self.unknown_sub = self.node.create_subscription(String, '/ui/unknown_username', self.get_unknown_name_callback, 10)
        self.need_navigate = False     
        self.name_pub = self.node.create_publisher(String, '/ui/username', 10)   
        self.face_recog_trig = self.node.create_publisher(String, '/face_recog_request', 10) 
        self.face_recog_sub= self.node.create_subscription(String, '/face_recog_result', self.get_name_callback, 10)
        self.unknown_name = None
        self.name = None
        self.angle = None

    def get_name_callback(self,msg):
        self.name,self.angle = msg.data.split(',')


    def get_unknown_name_callback(self,msg):
        self.unknown_name = msg.data

    def stop_listening_callback(self,msg):
        self.need_navigate = True

    def call_back(self,msg):
        self.trigger = True

    def execute(self, blackboard):
        print("Executing Conversation state")
        blackboard.gestures_obj.do_gesture(GestureType.AYUBOWAN)
        time.sleep(1)
        blackboard.conv_obj.blocking_tts("Aayuboawan. Wish you a happy new year. "+random.choice(welcoming_messages))    
        blackboard.conv_obj.blocking_tts(random.choice(waiting_messages))
        self.face_recog_trig.publish(String())
        while self.name == None:
            pass

        msg = String()
        msg.data = self.name
        self.name_pub.publish(msg)
        name ='unknown'
        if self.name!= 'unknown':
            blackboard.conv_obj.blocking_tts("Hi"+self.name + "Nice to see you again.")
            self.name = None
            self.angle = None
        else:
            blackboard.conv_obj.blocking_tts("We haven't met before. Could i know your name please? If you dont mind. Or you can skip.")
            self.name = None
            self.angle = None
            while self.unknown_name == None:
                pass
            print("Came out")
            if self.unknown_name!='<SKIP>':
                print("Not skip")
                blackboard.conv_obj.blocking_tts("Hi"+self.unknown_name+ ". Welcome to the Department of Electronic and Telecommuication Engineering.")
            self.unknown_name = None

        blackboard.conv_obj.start_listening()

        print("Exit from conversation state")
        
        if self.need_navigate:
            self.need_navigate = False
            blackboard.conv_obj.blocking_tts("Follow me")
            return "guide"
        else:
            return "end"
    
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
        self.nav_goal_pub = self.node.create_publisher(String, '/nav_goal', 10)
        self.nav_result_outcomes ={0: "UNKNOWN", 1:"SUCCEEDED", 2:"CANCELED" ,3:"FAILED"}
        self.timer = None

    def nav_result_callback(self, msg):
        self.nav_result = msg.data

    def app_goal_callback(self, msg):
        print("Destination from APP Recieved")        
        if self.goal:
            print("Navigator is busy. Try again later")
            return
        self.goal = [ msg.pose.position.x, msg.pose.position.y ]  

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
        print("Executing state Navigation")
        
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
                blackboard.conv_obj.blocking_tts("You have reached the destination. Have a nice day.")
                time.sleep(4)
                self.go_back_home()
            else:
                self.goal = None

                print("EXIT SUCCESSFUL FROM NAVIGATION")
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

        
        sm.add_state("COVERSATION", Conversation(self),
                     transitions={"end": "IDLE",
                                  "guide": "SWITCHINGPOWERMODE"}
                                  )
        
        sm.add_state("SWITCHINGPOWERMODE", SwitchingPowerMode(),
                    transitions={'navigate': "NAVIGATION",
                                 'idle': "IDLE"
                                 }                     
                    )

        sm.add_state("NAVIGATION", Navigation(self),
                     transitions={SUCCEED: "IDLE",
                                  ABORT:"NAVIGATION",
                                  CANCEL:"SWITCHINGPOWERMODE"})    

        
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
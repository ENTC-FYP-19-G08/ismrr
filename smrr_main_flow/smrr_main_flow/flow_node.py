import rclpy
from rclpy.node import Node
from std_msgs.msg import String

# from .smrr_navigation import SMRRNavigation
# from .smrr_navigation import TaskResult
# from .smrr_gestures import SMRRGestures
# from .smrr_gestures import GestureType
# from .smrr_face_recognition import SMRRFaceRecogition
# from .load_locations import LoadLocations


from simple_node import Node

from yasmin import State
from yasmin import StateMachine
from yasmin_viewer import YasminViewerPub
import time

from yasmin_ros.basic_outcomes import SUCCEED, ABORT, CANCEL

# define state Idle
class Idle(State):
    def __init__(self):
        super().__init__(["trigger","end"])

    def execute(self, blackboard):
        print("Executing state Idle")
        time.sleep(3)
        blackboard.nav = True
        return "trigger"

# define state Conversation
class Conversation(State):
    def __init__(self):
        super().__init__(["navigate", "end"])

    def execute(self, blackboard):
        print("Executing state Conversation")
        time.sleep(3)
        return "navigate"

# define state Navigate
class Navigation(State):
    def __init__(self):
        super().__init__([SUCCEED, ABORT, CANCEL])

    def execute(self, blackboard):
        print("Executing state Navigation")
        time.sleep(3)
        return SUCCEED

# define state BackHome
class BackHome(State):
    def __init__(self):
        super().__init__([SUCCEED])
      
    def execute(self, blackboard):
        print("Executing state BackHome")
        time.sleep(3)
        blackboard.nav = False
        return SUCCEED

# define state SwitchingPowerMode
class SwitchingPowerMode(State):
    def __init__(self):
        super().__init__(['navigate', 'idle'])
      
    def execute(self, blackboard):
        print("Executing state SwitchingPowerMode")
        time.sleep(3)
        if blackboard.nav:
            return 'navigate'
        else:
            return 'idle'
        
class MainFlow(Node):
    def __init__(self):
        super().__init__('flow_node')

        # _ = LoadLocations(self)
        # nav_obj = SMRRNavigation(self)
        # gesture_obj = SMRRGestures(self)
        # face_recsog_obj = SMRRFaceRecogition(self)

   
        # create a state machine
        sm = StateMachine(outcomes=["END"])

        # add states
        sm.add_state("IDLE", Idle(),
                     transitions={"trigger": "COVERSATION",
                                  "end":"END"})
        
        sm.add_state("COVERSATION", Conversation(),
                     transitions={"end": "IDLE",
                                  "navigate": "SWITCHINGPOWERMODE"})
        
        sm.add_state("SWITCHINGPOWERMODE",SwitchingPowerMode(),
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
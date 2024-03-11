import rclpy
from rclpy.node import Node

from std_msgs.msg import Bool


class triggerNode(Node):

    def __init__(self):
        super().__init__('trigger')
        self.publisher_ = self.create_publisher(Bool, '/smrr/face_recog/trigger', 1)
        

    def trigger_func(self):
        msg = Bool()
        while 1:
            x = int(input("Enter trigger: "))
            if x>0:
                msg.data = True
                
            else:
                msg.data = False

            self.publisher_.publish(msg)
            



def main(args=None):
    rclpy.init(args=args)

    minimal_publisher = triggerNode()
    minimal_publisher.trigger_func()
    rclpy.spin(minimal_publisher)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    minimal_publisher.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()

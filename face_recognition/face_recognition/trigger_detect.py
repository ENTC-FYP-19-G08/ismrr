import rclpy
from rclpy.node import Node

# from std_msgs.msg import Bool
from face_recog_interfaces.srv import FaceRecogRequest

class triggerNode(Node):

    def __init__(self):
        super().__init__('trigger')
        # self.publisher_ = self.create_publisher(Bool, '/smrr/face_recog/trigger', 1)
        self.client = self.create_client(FaceRecogRequest, '/smrr/face_recog_srv')
        while not self.client.wait_for_service(timeout_sec=1.0):
            self.get_logger().info('service not available, waiting again...')
        self.req = FaceRecogRequest.Request()

    def trigger_func(self):
        
        while 1:
            x = int(input("Enter trigger: "))
            if x>0:
                self.req.name_request = True
                self.req.angle_request = True
                
            else:
                self.req.name_request = False
                self.req.angle_request = False


            self.future = self.client.call_async(self.req)
            rclpy.spin_until_future_complete(self, self.future)

            response = self.future.result()
            print(response.name,response.angle)
            



def main(args=None):
    rclpy.init(args=args)

    service_client = triggerNode()
    service_client.trigger_func()
    # rclpy.spin(service_client)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    service_client.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()

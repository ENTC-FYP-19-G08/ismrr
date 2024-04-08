from enum import Enum

import rclpy
import serial
from face_recog_interfaces.srv import FaceRecogRequest


class SMRRFaceRecogition():

    def __init__(self, node):
        
        self.node = node
        self.client = self.node.create_client(FaceRecogRequest, '/smrr/face_recog_srv')
        while not self.client.wait_for_service(timeout_sec=1.0):
            self.node.get_logger().info('service not available, waiting again...')
        self.req = FaceRecogRequest.Request()

    
    def send_request(self):
        self.req.name_request = True
        self.req.angle_request = True
        self.future = self.client.call_async(self.req)
        rclpy.spin_until_future_complete(self.node, self.future)
        return self.future.result()
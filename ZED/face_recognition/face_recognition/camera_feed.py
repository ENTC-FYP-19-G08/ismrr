import rclpy
from rclpy.node import Node

from std_msgs.msg import String
from sensor_msgs.msg import Image

import cv2 as cv
import numpy as np

class framePublisher(Node):

    def __init__(self):
        super().__init__('frame_publisher')
        self.publisher = self.create_publisher(Image, '/zed/zed_node/left/image_rect_color', 2)
        self.camera_ID = 0
        self.camera = cv.VideoCapture(self.camera_ID) 

    def publishFrame(self):
        while True:
            result, video_frame = self.camera.read()  
            msg = Image()
            msg.header.stamp = Node.get_clock(self).now().to_msg()
            msg.header.frame_id = 'camera'
            msg.height = np.shape(video_frame)[0]
            msg.width = np.shape(video_frame)[1]
            msg.encoding = "bgr8"
            msg.is_bigendian = False
            msg.step = np.shape(video_frame)[2] * np.shape(video_frame)[1]

            msg.data = np.array(video_frame).tobytes()
            self.publisher.publish(msg)


def main(args=None):
    rclpy.init(args=args)

    frame_publisher = framePublisher()

    frame_publisher.publishFrame()

    rclpy.spin(frame_publisher)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    frame_publisher.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()

########################################################################
#
# Copyright (c) 2022, STEREOLABS.
#
# All rights reserved.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
########################################################################

"""
    This sample shows how to detect objects and draw 3D bounding boxes around them
    in an OpenGL window
"""
import sys
import ogl_viewer.viewer as gl
import pyzed.sl as sl
import argparse
import rclpy
from rclpy.node import Node
from visualization_msgs.msg import MarkerArray, Marker
import time
from tf2_ros import TransformException
from tf2_ros.buffer import Buffer
from tf2_ros.transform_listener import TransformListener
import asyncio
from tf2_geometry_msgs import do_transform_point
from geometry_msgs.msg import PointStamped,Point
from sensor_msgs.msg import PointCloud2
from sensor_msgs.msg import PointField 
import cv2

class ObjectsPublisher(Node):
    def __init__(self):
        super().__init__('objects_publisher_node')

        self.zed = sl.Camera()

        # Create a InitParameters object and set configuration parameters
        init_params = sl.InitParameters()
        init_params.coordinate_units = sl.UNIT.METER
        init_params.coordinate_system = sl.COORDINATE_SYSTEM.RIGHT_HANDED_Y_UP
        init_params.camera_resolution = sl.RESOLUTION.HD720
        # Open the camera
        err = self.zed.open(init_params)
        if err != sl.ERROR_CODE.SUCCESS:
            exit(1)

        # Enable object detection module
        obj_param = sl.ObjectDetectionParameters()
        # Defines if the object detection will track objects across images flow.
        obj_param.enable_tracking = True       # if True, enable positional tracking
        obj_param.detection_model = sl.OBJECT_DETECTION_MODEL.MULTI_CLASS_BOX_MEDIUM

        if obj_param.enable_tracking:
            self.zed.enable_positional_tracking()
            
        self.zed.enable_object_detection(obj_param)

        camera_info = self.zed.get_camera_information()
        
        # Create OpenGL viewer
        # self.viewer = gl.GLViewer()
        # self.viewer.init(camera_info.camera_configuration.calibration_parameters.left_cam, obj_param.enable_tracking)

        # Configure object detection runtime parameters
        self.obj_runtime_param = sl.ObjectDetectionRuntimeParameters()
        self.obj_runtime_param.detection_confidence_threshold = 60
        self.obj_runtime_param.object_class_filter = [sl.OBJECT_CLASS.PERSON]    # Only detect Persons

        # Create ZED objects filled in the main loop
        self.objects = sl.Objects()
        self.image = sl.Mat()
        self.point_cloud = sl.Mat()

        # Set runtime parameters
        self.runtime_parameters = sl.RuntimeParameters()

        self.publisher_ = self.create_publisher(MarkerArray, '/objects', 1)
        self.pc_publisher_ = self.create_publisher(PointCloud2, '/camera_point_cloud', 1)
        self.tf_buffer = Buffer()
        self.tf_listener = TransformListener(self.tf_buffer, self)

        # self.create_timer(0.4, self.detect_objects)
        self.create_timer(1.0, self.get_point_cloud)

    def get_point_cloud(self):
        if self.zed.grab(self.runtime_parameters) == sl.ERROR_CODE.SUCCESS:

            # Retrieve point_cloud
            self.zed.retrieve_measure(self.point_cloud, sl.MEASURE.XYZRGBA)
            
            pc_msg = PointCloud2()
            pc_msg.width = self.point_cloud.get_width()
            pc_msg.height = self.point_cloud.get_height()

            pc_msg.header.stamp = self.get_clock().now().to_msg()
            pc_msg.header.frame_id = "zed2_left_camera_frame"
            pc_msg.is_bigendian = False
            pc_msg.is_dense = False
            pc_msg.fields , off_set = self.get_fields()

            pc_msg.point_step = off_set
            pc_msg.row_step = pc_msg.width * pc_msg.point_step
            
        
            print(pc_msg.row_step*pc_msg.height)
            print(self.point_cloud.get_channels)
            print(self.point_cloud.read)
            print(self.point_cloud.get_data_type)
            print(self.point_cloud.move)
            # pc_msg.data = self.point_cloud.get_data()
            # pc_publisher_.publish(pc_msg)

    def get_fields(self):
        arr=[]

        off_set = 0
        fields = [ ("x", 1, PointField.FLOAT32), 
                 ("y", 1,PointField.FLOAT32), 
                 ("z", 1, PointField.FLOAT32),
                 ("rgb",1, PointField.FLOAT32)]

        for f in fields:
            point_field = PointField()
            point_field.name = f[0]
            point_field.count = f[1]
            point_field.datatype = f[2]
            point_field.offset = off_set
            arr.append(point_field)
            off_set += point_field.count * 4

        return arr,off_set

    def detect_objects(self):
        if self.zed.grab(self.runtime_parameters) == sl.ERROR_CODE.SUCCESS and self.viewer.is_available():

            # Retrieve left image
            self.zed.retrieve_image(self.image, sl.VIEW.LEFT)   

            image_ocv = self.image.get_data()
            # Display the left image from the numpy array
            cv2.imshow("Image", image_ocv)
            key = cv2.waitKey(10)

            # Retrieve objects
            self.zed.retrieve_objects(self.objects, self.obj_runtime_param)
            if self.objects.is_new:
                obj_array = self.objects.object_list
                print(str(len(obj_array))+" Object(s) detected\n")
                if len(obj_array) > 0 :
                    self.publish_data(obj_array)       
            # # Update GL view
            self.viewer.update_view(self.image, self.objects)   

  

    def publish_data(self,array):
        objects = MarkerArray()
        id = 1

       
        time = self.get_clock().now().to_msg()

        none_object = Marker()
        none_object.header.stamp = time
        none_object.header.frame_id = "zed2_left_camera_frame"
        none_object.id = 0
        none_object.action = 3
        objects.markers.append(none_object)

        for obj in array:
            print(" Label '"+repr(obj.label)+"' (conf. "+str(int(obj.confidence))+"/100) "+str(obj.id))
      
            position = obj.position
            velocity = obj.velocity
            dimensions = obj.dimensions

            single_object = Marker()
            single_object.header.stamp = time
            single_object.header.frame_id = "zed2_left_camera_frame"
            single_object.id = id

            single_object.pose.position.x =  float(-position[2])
            single_object.pose.position.y =  float(-position[0])
            single_object.pose.position.z =  float(position[1])
            single_object.scale.x =  0.2
            single_object.scale.y =  0.2
            single_object.scale.z =  0.2
            single_object.type = 3
            single_object.color.r = 0.0
            single_object.color.g = 0.0
            single_object.color.b = 1.0
            single_object.color.a = 1.0
            objects.markers.append(single_object)

            id +=1   
            text = Marker()
            text.header.stamp = self.get_clock().now().to_msg()
            text.header.frame_id = "zed2_left_camera_frame"
            text.id = id
            text.pose.position.x =  float(-position[2])
            text.pose.position.y =  float(-position[0])+0.1
            text.pose.position.z =  float(position[1])
            text.scale.x =  0.2
            text.scale.y =  0.2
            text.scale.z =  0.2
            text.type = 9
            text.color.r = 0.0
            text.color.g = 1.0
            text.color.b = 0.0
            text.color.a = 1.0
            text.text = str(obj.label)+str(obj.id)
            objects.markers.append(text)
            id +=1    

        self.publisher_.publish(objects)     


def main():
    rclpy.init()
    objects_publisher = ObjectsPublisher()
    rclpy.spin(objects_publisher)

    objects_publisher.image.free(memory_type=sl.MEM.CPU)
    objects_publisher.zed.disable_object_detection()
    objects_publisher.zed.disable_positional_tracking()
    objects_publisher.zed.close()
    objects_publisher.viewer.exit()

    objects_publisher.destroy_node()
    rclpy.shutdown()

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--resolution', type=str, help='Resolution, can be either HD2K, HD1200, HD1080, HD720, SVGA or VGA', default = 'VGA')
    opt = parser.parse_args()
    main() 


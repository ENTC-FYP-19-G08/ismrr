from visualization_msgs.msg import MarkerArray
from visualization_msgs.msg import Marker

from rclpy.qos import QoSProfile
from rclpy.qos import ReliabilityPolicy, DurabilityPolicy, HistoryPolicy

class LoadLocations():
    def __init__(self, node):        
        self.node = node
        qos = QoSProfile(depth=10)
        qos.reliability = ReliabilityPolicy.RMW_QOS_POLICY_RELIABILITY_RELIABLE
        qos.durability = DurabilityPolicy.RMW_QOS_POLICY_DURABILITY_TRANSIENT_LOCAL
        qos.history = HistoryPolicy.RMW_QOS_POLICY_HISTORY_KEEP_ALL
        self.location_pub = self.node.create_publisher(MarkerArray, '/locations', qos)
        self.locations = dict()
        self.load_locations()
        self.show_locations()

    def load_locations(self):
        f = open("/SSD/ros2_ws/src/Locations/locations.txt", "r")
        for item in f.read().splitlines():
            split_items = item.split(',')
            key = split_items[0]
            values = split_items[1:]  
            paired_values = [(float(values[i]), float(values[i+1])) for i in range(0, len(values), 2)]
            self.locations[key] = paired_values[0]
        f.close()
        print("Locations were loaded successfully")
    
    def show_locations(self):
        marker_array = MarkerArray()
        id_ = 1
        for key, value in self.locations.items():
            marker = Marker()
            marker.header.frame_id = "map"
            marker.header.stamp = self.node.get_clock().now().to_msg()
            marker.id = id_
            marker.pose.position.x = float(value[0])
            marker.pose.position.y = float(value[1])
            marker.type = Marker.CUBE
            marker.scale.x =  0.4
            marker.scale.y =  0.4
            marker.scale.z =  0.4
            marker.color.r = 0.0
            marker.color.g = 0.0
            marker.color.b = 1.0
            marker.color.a = 1.0
            marker_array.markers.append(marker)
            id_ = id_+1

            marker2 = Marker()
            marker2.header.frame_id = "map"
            marker2.header.stamp = self.node.get_clock().now().to_msg()
            marker2.id = id_
            marker2.pose.position.x = float(value[0])+0.34
            marker2.pose.position.y = float(value[1])
            marker2.type = Marker.TEXT_VIEW_FACING
            marker2.scale.x =  0.5
            marker2.scale.y =  0.5
            marker2.scale.z =  0.5
            marker2.color.r = 1.0
            marker2.color.g = 0.0
            marker2.color.b = 1.0
            marker2.color.a = 1.0
            marker2.text = key
            marker_array.markers.append(marker2)
            id_ = id_+1
            # print(id_,key, value)

        self.location_pub.publish(marker_array)
        # navigation_result = nav_obj.reach_destination(self.locations["Entc1"])
        # if navigation_result == TaskResult.SUCCEEDED:
        #     print('Done')
        # print(face_recog_obj.send_request())


        
        # self.publisher_ = self.create_publisher(String, 'topic', 10)
        # timer_period = 0.5  # seconds
        # self.timer = self.create_timer(timer_period, self.timer_callback)
        # self.i = 0


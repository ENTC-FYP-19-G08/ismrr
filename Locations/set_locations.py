import rclpy
from rclpy.node import Node
from std_msgs.msg import String
from geometry_msgs.msg import PointStamped

class MainFlow(Node):
    def __init__(self):
        super().__init__('location_saver')

        self.sub = self.create_subscription(PointStamped, '/clicked_point',self.rviz_callback,1)
        # self.publisher_ = self.create_publisher(String, 'topic', 10)
        # timer_period = 0.5  # seconds
        # self.timer = self.create_timer(timer_period, self.timer_callback)
        # self.i = 0

    def rviz_callback(self, msg):
        x = input(f"Enter the location of {round(msg.point.x,2)} {round(msg.point.y,2)} : ")

        f = open("/SSD/ros2_ws/src/Locations/locations.txt", "a")
        f.write(f"{x},{round(msg.point.x,2)},{round(msg.point.y,2)}\n")
        f.close()

        print("saved")

def main(args=None):
    rclpy.init(args=args)

    main_flow = MainFlow()

    rclpy.spin(main_flow)

    main_flow.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
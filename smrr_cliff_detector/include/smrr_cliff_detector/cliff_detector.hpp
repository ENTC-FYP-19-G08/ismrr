#include <chrono>
#include <memory>
#include <string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<iomanip>
#include<string>
#include<cstdlib>
#include <cmath>

#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "std_msgs/msg/string.hpp"
#include <tf2/LinearMath/Quaternion.h>
#include "tf2/exceptions.h"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"
#include "tf2_sensor_msgs/tf2_sensor_msgs.h"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "sensor_msgs/msg/point_cloud2.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "sensor_msgs/point_cloud2_iterator.hpp"
using namespace std::chrono_literals;

//#define DEBUG

class CliffDetector : public rclcpp::Node
{
  public:
    CliffDetector();

    
  private:
    void debug_param();
    void filter_noise();
    void callback(const sensor_msgs::msg::PointCloud2::SharedPtr cloud_msg);

    rclcpp::Publisher<sensor_msgs::msg::LaserScan>::SharedPtr pub_;
    rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr point_cloud_subscriber_;


    // rclcpp::TimerBase::SharedPtr timer_, timer2_, timer3_;
    
    std::shared_ptr<tf2_ros::TransformListener> tf_listener_{nullptr};
    std::unique_ptr<tf2_ros::Buffer> tf_buffer_;

    std::string point_cloud_topic_;
    float camera_z_offset_,transform_tolerance_,ground_level_,angle_min_,angle_max_,angle_increment_,scan_time_,range_min_,range_max_,inf_epsilon_;
    bool use_inf_;
    int odom_publish_rate_,tf_publish_rate_;

}; 

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
#include "std_msgs/msg/string.hpp"
#include <tf2/LinearMath/Quaternion.h>
#include "tf2_ros/transform_broadcaster.h"
#include "geometry_msgs/msg/transform_stamped.hpp"

using namespace std::chrono_literals;

//#define DEBUG

class DiffDriveController : public rclcpp::Node
{
  public:
    DiffDriveController();

    
  private:
    void debug_param();
    void publish_odom();
    void publish_tf();
    void calculate_odom();
    void callback(const std_msgs::msg::String::SharedPtr msg) ;

    rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr odom_publisher_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr in_velocity_subscriber_;

    std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;

    rclcpp::TimerBase::SharedPtr timer_, timer2_, timer3_;
    
    float wheel_separation_, wheel_radius_;

    float left_velocity_,right_velocity_ ;
    float position_x_ , position_y_;
    float yaw_angle_;

    int odom_publish_rate_,tf_publish_rate_;
}; 


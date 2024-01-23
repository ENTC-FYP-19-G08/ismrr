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
    void velocity_callback(const geometry_msgs::msg::Twist::SharedPtr msg) ;

    rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr odom_publisher_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr wheel_vel_publisher_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr encoder_subscriber_;
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_subscriber_;
    std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;

    rclcpp::TimerBase::SharedPtr timer_, timer2_, timer3_;
    
    int counts_per_rotation_left_,counts_per_rotation_right_;
    float wheel_separation_, wheel_radius_, rotation_per_count_;

    int left_wheel_odom_,right_wheel_odom_, prev_left_wheel_odom_, prev_right_wheel_odom_;
    float position_x_ , position_y_;
    float yaw_angle_;
    float linear_vel_x_, angular_velocity_z;

    int odom_publish_rate_,tf_publish_rate_;
}; 


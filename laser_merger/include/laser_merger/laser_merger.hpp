#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include <math.h>

class LaserMerger : public rclcpp::Node
{
  public:
    LaserMerger();

  private:
    void scan1_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg);
    void scan2_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg);
    void timer_callback();
    
    rclcpp::Publisher<sensor_msgs::msg::LaserScan>::SharedPtr merged_scan_publisher_;
    rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr scan1_subscriber_;
    rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr scan2_subscriber_;
     rclcpp::TimerBase::SharedPtr timer_;

    sensor_msgs::msg::LaserScan::SharedPtr scan2 = nullptr;
    double min_angle_;
    double max_angle_;
    int elasped_time;    
};

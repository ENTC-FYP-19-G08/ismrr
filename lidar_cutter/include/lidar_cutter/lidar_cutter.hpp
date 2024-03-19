#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include <math.h>

class LidarCutter : public rclcpp::Node
{
  public:
    LidarCutter();
    

  private:
    void scan_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg);
    
    rclcpp::Publisher<sensor_msgs::msg::LaserScan>::SharedPtr cut_scan_publisher_;
    rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr scan_subscriber_;
  
    double min_angle_;
    double max_angle_;
};

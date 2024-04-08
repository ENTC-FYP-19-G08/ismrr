#include <lidar_cutter/lidar_cutter.hpp>

using std::placeholders::_1;

LidarCutter:: LidarCutter() : 
    Node("lidar_cutter_node")
{   
    this->declare_parameter("min_angle",-1*M_PI);
    this->declare_parameter("max_angle", M_PI);
    
    min_angle_ = this->get_parameter("min_angle").get_parameter_value().get<double>();
    max_angle_ = this->get_parameter("max_angle").get_parameter_value().get<double>();

    cut_scan_publisher_ = this->create_publisher<sensor_msgs::msg::LaserScan>("/scan", rclcpp::QoS(rclcpp::SensorDataQoS()));
    scan_subscriber_ = this->create_subscription<sensor_msgs::msg::LaserScan>("/scan_raw", rclcpp::QoS(rclcpp::SensorDataQoS()), std::bind(&LidarCutter::scan_callback, this, _1));


}

void LidarCutter::scan_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg){
    
    int start_index = (min_angle_ -  msg->angle_min)/msg->angle_increment;
    int end_index =   ( max_angle_ -  msg->angle_min)/msg->angle_increment ;

    std::vector<float> cut_ranges(msg->ranges.begin() + start_index, msg->ranges.begin() + end_index + 1);
    std::vector<float> cut_intensities(msg->intensities.begin() + start_index, msg->intensities.begin() + end_index + 1);

    std::fill(msg->ranges.begin() + start_index, msg->ranges.begin() + end_index + 1, std::numeric_limits<float>::infinity());
    std::fill(msg->intensities.begin() + start_index, msg->intensities.begin() + end_index + 1, 0.0f);

    sensor_msgs::msg::LaserScan scan;
    scan.angle_increment = msg->angle_increment;
    scan.angle_max = msg->angle_max;
    scan.angle_min = msg->angle_min;
    scan.header.frame_id = msg->header.frame_id;
    scan.header.stamp = msg->header.stamp;
    scan.intensities = msg->intensities;
    scan.range_max = msg->range_max;
    scan.range_min = msg->range_min;
    scan.ranges = msg->ranges;
    scan.scan_time = msg->scan_time;

    cut_scan_publisher_->publish(scan);
    

}    
#include <laser_merger/laser_merger.hpp>

using std::placeholders::_1;
using namespace std::chrono_literals;

LaserMerger:: LaserMerger() : 
    Node("laser_merger_node")
{   

    this->declare_parameter("min_angle",-1*M_PI);
    this->declare_parameter("max_angle", M_PI);
    
    min_angle_ = this->get_parameter("min_angle").get_parameter_value().get<double>();
    max_angle_ = this->get_parameter("max_angle").get_parameter_value().get<double>();

    merged_scan_publisher_ = this->create_publisher<sensor_msgs::msg::LaserScan>("/scan", rclcpp::QoS(rclcpp::SensorDataQoS()));
    scan1_subscriber_ = this->create_subscription<sensor_msgs::msg::LaserScan>("/scan_raw", rclcpp::QoS(rclcpp::SensorDataQoS()), std::bind(&LaserMerger::scan1_callback, this, _1));
    scan2_subscriber_ = this->create_subscription<sensor_msgs::msg::LaserScan>("/scan_cloud", rclcpp::QoS(rclcpp::SensorDataQoS()), std::bind(&LaserMerger::scan2_callback, this, _1));
  
    timer_ = this->create_wall_timer(2s, std::bind(&LaserMerger::timer_callback, this));
}

void LaserMerger::timer_callback()
{
    elasped_time +=2;
    if (elasped_time >5){
        scan2 = nullptr;
    }

}
void LaserMerger::scan1_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg){
    
    // std::cout<<"scan1 recieved"<<std::endl;

    int start_index = (min_angle_ -  msg->angle_min)/msg->angle_increment;
    int end_index =   ( max_angle_ -  msg->angle_min)/msg->angle_increment ;

    std::vector<float> cut_ranges(msg->ranges.begin() + start_index, msg->ranges.begin() + end_index + 1);
    std::vector<float> cut_intensities(msg->intensities.begin() + start_index, msg->intensities.begin() + end_index + 1);

    std::fill(msg->ranges.begin() + start_index, msg->ranges.begin() + end_index + 1, std::numeric_limits<float>::infinity());
    std::fill(msg->intensities.begin() + start_index, msg->intensities.begin() + end_index + 1, 0.0f);
    
    sensor_msgs::msg::LaserScan merged;
    merged.angle_increment = msg->angle_increment;
    merged.angle_max = msg->angle_max;
    merged.angle_min = msg->angle_min;
    merged.header.frame_id = msg->header.frame_id;
    merged.header.stamp = msg->header.stamp;
    merged.intensities = msg->intensities;
    merged.range_max = msg->range_max;
    merged.range_min = msg->range_min;
    merged.ranges = msg->ranges;
    merged.scan_time = msg->scan_time;  

    if (scan2 == nullptr){
        merged_scan_publisher_->publish(merged);
        return;
    }

    for (size_t i = 0; i < std::min(msg->ranges.size(), scan2->ranges.size()); ++i) {

        if (merged.ranges[i] > scan2->ranges[i])
            merged.ranges[i] = scan2->ranges[i];
            merged.intensities[i] = 47.0;
    }

    merged_scan_publisher_->publish(merged);

}

void LaserMerger::scan2_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg){
    elasped_time = 0;
    scan2 = msg;
}
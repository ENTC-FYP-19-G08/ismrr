#include "imra_diff_drive_controller/controller.hpp"

using std::placeholders::_1;

DiffDriveController:: DiffDriveController() : 
    Node("diff_drive_controller_node"),
    left_velocity_(0.0),
    right_velocity_(0.0),
    position_x_(0.0),
    position_y_(0.0),
    yaw_angle_(0)

{   
    // std::string nodename=DiffDriveController::get_name();

    this->declare_parameter("wheel_separation", 1.0);
    wheel_separation_ =this->get_parameter("wheel_separation").get_parameter_value().get<float>();
    this->declare_parameter("wheel_radius", 1.0);
    wheel_radius_ = this->get_parameter("wheel_radius").get_parameter_value().get<float>(); 
    this->declare_parameter("odom_publish_rate", 1);
    odom_publish_rate_ =this->get_parameter("odom_publish_rate").get_parameter_value().get<int>();
    this->declare_parameter("tf_publish_rate", 1);
    tf_publish_rate_ =this->get_parameter("tf_publish_rate").get_parameter_value().get<int>();
 


    //Publishers 
    odom_publisher_ = this->create_publisher<nav_msgs::msg::Odometry>("/test_odom", 100);


    debug_param();
    in_velocity_subscriber_ = this->create_subscription<std_msgs::msg::String>("/test_velocity", 4, std::bind(&DiffDriveController::callback, this, _1));
    
    // this->client_ptr_ = rclcpp_action::create_client<nav2_msgs::action::NavigateToPose>(this, ns + "/navigate_to_pose");
    tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);

    timer_ = this->create_wall_timer(10ms, std::bind(&DiffDriveController::calculate_odom, this));
    timer2_ = this->create_wall_timer(std::chrono::milliseconds(int (1000/odom_publish_rate_)), std::bind(&DiffDriveController::publish_odom, this));
    timer3_ = this->create_wall_timer(std::chrono::milliseconds(int (1000/tf_publish_rate_)), std::bind(&DiffDriveController::publish_tf, this));

    
}

void DiffDriveController::callback(const std_msgs::msg::String::SharedPtr msg)
{
    std::string inputString = msg->data;

    std::istringstream iss(inputString);

    if (iss >> left_velocity_ >> right_velocity_) {
        // Successfully extracted two float numbers
        std::cout << "left_velocity_: " << left_velocity_ << ", right_velocity_: " << right_velocity_ << std::endl;
    } else {
        // Extraction failed
        std::cerr << "Failed to extract float numbers from the string." << std::endl;
    }   
}

void DiffDriveController::calculate_odom(){

    float left_distance = left_velocity_ * 0.05;
    float right_distance = right_velocity_ * 0.05;

    float alpha = (right_distance-left_distance)/wheel_separation_ ; 

    if (abs(alpha)>0){
        float radius =  (left_distance / alpha);

        float center_of_rotation_x = position_x_ - (radius+wheel_separation_/2)*sin(yaw_angle_);
        float center_of_rotation_y = position_y_ + (radius+wheel_separation_/2)*cos(yaw_angle_); //-cos 
        
        yaw_angle_ = fmod(yaw_angle_+alpha, 2*M_PI);

        position_x_ = center_of_rotation_x + (radius+wheel_separation_/2)*sin(yaw_angle_);
        position_y_ = center_of_rotation_y - (radius+wheel_separation_/2)*cos(yaw_angle_); //-cos 
    }

    else{
        position_x_ = position_x_ + left_distance*cos(yaw_angle_);
        position_y_ = position_y_ + left_distance*sin(yaw_angle_);
    }
 
}

void DiffDriveController::publish_odom(){

    tf2::Quaternion q;
    // std::cout<< yaw_angle_<<std::endl;
    q.setRPY(0, 0, yaw_angle_);
    q.normalize();

    nav_msgs::msg::Odometry msg;
    msg.header.frame_id = "/odom";
    msg.header.stamp = this->get_clock()->now();
    msg.pose.pose.position.x = position_x_;
    msg.pose.pose.position.y = position_y_;
    msg.pose.pose.orientation.x = q.x();
    msg.pose.pose.orientation.y = q.y();
    msg.pose.pose.orientation.z = q.z();
    msg.pose.pose.orientation.w = q.w();

    odom_publisher_->publish(msg);

}

void DiffDriveController::publish_tf(){

    geometry_msgs::msg::TransformStamped t;


    t.header.stamp = this->get_clock()->now();
    t.header.frame_id = "/odom";
    t.child_frame_id = "/base_footprint";

    t.transform.translation.x = position_x_;
    t.transform.translation.y = position_y_;
    t.transform.translation.z = 0.0;

    tf2::Quaternion q;
    q.setRPY(0, 0, yaw_angle_);
    q.normalize();
    t.transform.rotation.x = q.x();
    t.transform.rotation.y = q.y();
    t.transform.rotation.z = q.z();
    t.transform.rotation.w = q.w();

    tf_broadcaster_->sendTransform(t);


}

void DiffDriveController::debug_param(){
      RCLCPP_INFO_STREAM(DiffDriveController::get_logger(), "wheel_separation " << wheel_separation_
      <<"\nwheel_radius: "<<wheel_radius_
      );


}

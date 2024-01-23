#include "smrr_diff_drive_controller/controller.hpp"

using std::placeholders::_1;

DiffDriveController:: DiffDriveController() : 
    Node("diff_drive_controller_node"),
    left_wheel_odom_(0),
    right_wheel_odom_(0),
    prev_left_wheel_odom_(0),
    prev_right_wheel_odom_(0),
    position_x_(0.0f),
    position_y_(0.0f),
    yaw_angle_(0),
    linear_vel_x_(0.0f),
    angular_velocity_z(0.0f)

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
    
    this->declare_parameter("counts_per_rotation_left", 95);
    counts_per_rotation_left_ = this->get_parameter("counts_per_rotation_left").get_parameter_value().get<int>();
    this->declare_parameter("counts_per_rotation_right", 95);
    counts_per_rotation_right_ = this->get_parameter("counts_per_rotation_right").get_parameter_value().get<int>();
    

    
    //Publishers 
    odom_publisher_ = this->create_publisher<nav_msgs::msg::Odometry>("/wheel_odom", 100);
    wheel_vel_publisher_ = this->create_publisher<std_msgs::msg::String>("/wheel_velocity", 100);

    debug_param();

    //Subscribers
    encoder_subscriber_ = this->create_subscription<std_msgs::msg::String>("/wheel_encoders", 1, std::bind(&DiffDriveController::callback, this, _1));
    cmd_vel_subscriber_ = this->create_subscription<geometry_msgs::msg::Twist>("/cmd_vel_smoothen", 1, std::bind(&DiffDriveController::velocity_callback, this, _1));

    // this->client_ptr_ = rclcpp_action::create_client<nav2_msgs::action::NavigateToPose>(this, ns + "/navigate_to_pose");
    tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);

    timer_ = this->create_wall_timer(20ms, std::bind(&DiffDriveController::calculate_odom, this));
    timer2_ = this->create_wall_timer(std::chrono::milliseconds(int (1000/odom_publish_rate_)), std::bind(&DiffDriveController::publish_odom, this));
    timer3_ = this->create_wall_timer(std::chrono::milliseconds(int (1000/tf_publish_rate_)), std::bind(&DiffDriveController::publish_tf, this));

    
}

void DiffDriveController::velocity_callback(const geometry_msgs::msg::Twist::SharedPtr msg)
{
    std_msgs::msg::String vel;

    float l_speed = msg->linear.x;
    float r_speed = msg->linear.x;

    r_speed += (msg->angular.z) * wheel_separation_/2;
    l_speed -= (msg->angular.z) * wheel_separation_/2;

    vel.data = std::to_string(l_speed) + " " + std::to_string(r_speed);

    wheel_vel_publisher_ -> publish(vel);
}

void DiffDriveController::callback(const std_msgs::msg::String::SharedPtr msg)
{
    std::string inputString = msg->data;

    std::istringstream iss(inputString);

    if (iss >> left_wheel_odom_ >> right_wheel_odom_) {
        // Successfully extracted two float numbers
        // std::cout << "left_wheel_odom_: " << left_wheel_odom_ << ", right_wheel_odom_: " << right_wheel_odom_ << std::endl;
    } else {
        // Extraction failed
        std::cerr << "Failed to extract float numbers from the string." << std::endl;
    }   
}

void DiffDriveController::calculate_odom(){

    int corrected_left_wheel_odom = left_wheel_odom_; 
    int corrected_right_wheel_odom = right_wheel_odom_;


    if ((prev_left_wheel_odom_> 800) && (left_wheel_odom_< 800)){corrected_left_wheel_odom += 1000;}
    if ((prev_right_wheel_odom_> 800) && (right_wheel_odom_< 800)){corrected_right_wheel_odom += 1000;}
    
    if ((prev_left_wheel_odom_< -800) && (left_wheel_odom_> -200)){corrected_left_wheel_odom -= 1000;}
    if ((prev_right_wheel_odom_< -800) && (right_wheel_odom_> -200)){corrected_right_wheel_odom -= 1000;}
    
    float left_distance = (corrected_left_wheel_odom - prev_left_wheel_odom_)*(1.0/counts_per_rotation_left_)*M_PI*2*wheel_radius_ ;
    float right_distance = (corrected_right_wheel_odom - prev_right_wheel_odom_)*(1.0/counts_per_rotation_right_)*M_PI*2*wheel_radius_;

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

    float dt= 0.020f;
    linear_vel_x_ = (left_distance+right_distance)/(2*dt);
    angular_velocity_z = alpha/dt;

    // std::cout << "linear_vel_x_: " << linear_vel_x_ << ", angular_velocity_z: " << angular_velocity_z << std::endl;

    prev_left_wheel_odom_ = left_wheel_odom_;
    prev_right_wheel_odom_ = right_wheel_odom_;
 
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

    msg.twist.twist.linear.x = linear_vel_x_;
    msg.twist.twist.angular.z = angular_velocity_z;

    odom_publisher_->publish(msg);

}

void DiffDriveController::publish_tf(){

    geometry_msgs::msg::TransformStamped t;


    t.header.stamp = this->get_clock()->now();
    t.header.frame_id = "/odom";
    t.child_frame_id = "/base_link";

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

    // std::cout << "position_x_: " << position_x_ << ", position_y_: " << position_y_ << std::endl;


}

void DiffDriveController::debug_param(){
      RCLCPP_INFO_STREAM(DiffDriveController::get_logger(), "wheel_separation " << wheel_separation_
      <<"\nwheel_radius: "<<wheel_radius_<<"\ncounts_per_rotation_left: "<<counts_per_rotation_left_<<"\ncounts_per_rotation_right: "<<counts_per_rotation_right_
      );

    

}

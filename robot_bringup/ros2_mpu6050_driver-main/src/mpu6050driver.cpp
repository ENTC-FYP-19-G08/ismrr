#include "mpu6050driver/mpu6050driver.h"

#include <chrono>
#include <memory>

using namespace std::chrono_literals;

MPU6050Driver::MPU6050Driver()
    : Node("mpu6050publisher"), mpu6050_{std::make_unique<MPU6050Sensor>()}
{
  // Declare parameters
  declareParameters();
  // Set parameters
  mpu6050_->setGyroscopeRange(
      static_cast<MPU6050Sensor::GyroRange>(this->get_parameter("gyro_range").as_int()));
  mpu6050_->setAccelerometerRange(
      static_cast<MPU6050Sensor::AccelRange>(this->get_parameter("accel_range").as_int()));
  mpu6050_->setDlpfBandwidth(
      static_cast<MPU6050Sensor::DlpfBandwidth>(this->get_parameter("dlpf_bandwidth").as_int()));
  mpu6050_->setGyroscopeOffset(this->get_parameter("gyro_x_offset").as_double(),
                               this->get_parameter("gyro_y_offset").as_double(),
                               this->get_parameter("gyro_z_offset").as_double());
  mpu6050_->setAccelerometerOffset(this->get_parameter("accel_x_offset").as_double(),
                                   this->get_parameter("accel_y_offset").as_double(),
                                   this->get_parameter("accel_z_offset").as_double());
  // Check if we want to calibrate the sensor
  if (this->get_parameter("calibrate").as_bool()) {
    RCLCPP_INFO(this->get_logger(), "Calibrating...");
    mpu6050_->calibrate();
  }
  mpu6050_->printConfig();
  mpu6050_->printOffsets();
  // Create publisher
  publisher_ = this->create_publisher<sensor_msgs::msg::Imu>("imu", 10);
  std::chrono::duration<int64_t, std::milli> frequency =
      1000ms / this->get_parameter("frequency").as_int();
  timer_ = this->create_wall_timer(frequency, std::bind(&MPU6050Driver::handleInput, this));
}

void MPU6050Driver::handleInput()
{
  auto message = sensor_msgs::msg::Imu();
  message.header.stamp = this->get_clock()->now();
  message.header.frame_id = "base_link";
    
  
  std::array<double, 9UL> linear_acc_matrix = {
    0.1, 0.0, 0.0,
    0.0, 0.1, 0.0,
    0.0, 0.0, 0.1 };

  message.linear_acceleration_covariance = linear_acc_matrix;
  message.linear_acceleration.x = mpu6050_->getAccelerationX();
  message.linear_acceleration.y = mpu6050_->getAccelerationY();
  message.linear_acceleration.z = mpu6050_->getAccelerationZ();
  message.angular_velocity_covariance[0] = {0};
  message.angular_velocity.x = mpu6050_->getAngularVelocityX() * M_PI/180.0;
  message.angular_velocity.y = mpu6050_->getAngularVelocityY() * M_PI/180.0;
  message.angular_velocity.z = mpu6050_->getAngularVelocityZ() * M_PI/180.0;
  // Invalidate quaternion

  // if (message.angular_velocity.x>0.001) roll += 0.01*message.angular_velocity.x;
  // if (message.angular_velocity.y>0.001) pitch += 0.01*message.angular_velocity.y;
  // if (message.angular_velocity.z>0.001) yaw += 0.01*message.angular_velocity.z; 

  tf2::Quaternion q;
  q.setRPY(roll, pitch, yaw);
  q.normalize();
  message.orientation_covariance[0] = {0};
  message.orientation.x = q.x();
  message.orientation.y = q.y();
  message.orientation.z = q.z();
  message.orientation.w = q.w();
  publisher_->publish(message);
  // std::cout<<"\rax: "<<message.linear_acceleration.x <<" ay: "<<message.linear_acceleration.y <<" az: "<<message.linear_acceleration.z <<std::endl;
  // std::cout<<"vx: "<<message.angular_velocity.x <<" vy: "<<message.angular_velocity.y <<" vz: "<<message.angular_velocity.z <<std::endl;
  // std::cout<<"roll: "<<roll <<" pitch: "<<pitch <<" yaw: "<<yaw <<std::endl;
  // std::cout << std::flush;
}

void MPU6050Driver::declareParameters()
{
  this->declare_parameter<bool>("calibrate", true);
  this->declare_parameter<int>("gyro_range", MPU6050Sensor::GyroRange::GYR_250_DEG_S);
  this->declare_parameter<int>("accel_range", MPU6050Sensor::AccelRange::ACC_2_G);
  this->declare_parameter<int>("dlpf_bandwidth", MPU6050Sensor::DlpfBandwidth::DLPF_260_HZ);
  this->declare_parameter<double>("gyro_x_offset", 0.0);
  this->declare_parameter<double>("gyro_y_offset", 0.0);
  this->declare_parameter<double>("gyro_z_offset", 0.0);
  this->declare_parameter<double>("accel_x_offset", 0.0);
  this->declare_parameter<double>("accel_y_offset", 0.0);
  this->declare_parameter<double>("accel_z_offset", 0.0);
  this->declare_parameter<int>("frequency", 0.0);
}

int main(int argc, char* argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MPU6050Driver>());
  rclcpp::shutdown();
  return 0;
}
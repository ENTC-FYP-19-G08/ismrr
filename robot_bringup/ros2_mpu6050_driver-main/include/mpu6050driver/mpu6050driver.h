#ifndef MPU6050DRIVER_H
#define MPU6050DRIVER_H

#include "mpu6050driver/mpu6050sensor.h"
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/imu.hpp"
#include <cmath>
#include <tf2/LinearMath/Quaternion.h>
class MPU6050Driver : public rclcpp::Node {
 public:
  MPU6050Driver();

 private:
  rclcpp::Publisher<sensor_msgs::msg::Imu>::SharedPtr publisher_;
  std::unique_ptr<MPU6050Sensor> mpu6050_;
  size_t count_;
  rclcpp::TimerBase::SharedPtr timer_;

  double roll = 0, pitch = 0, yaw = 0;
  void handleInput();
  void declareParameters();
};

#endif  // MPU6050DRIVER_H
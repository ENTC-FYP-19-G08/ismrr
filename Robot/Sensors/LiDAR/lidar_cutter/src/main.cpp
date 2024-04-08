#include <lidar_cutter/lidar_cutter.hpp>

using namespace std;

int main(int argc, char * argv[])
{
  
  rclcpp::init(argc, argv);
  auto node = std::make_shared<LidarCutter>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
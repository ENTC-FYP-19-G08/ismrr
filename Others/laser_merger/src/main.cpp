#include <laser_merger/laser_merger.hpp>

using namespace std;

int main(int argc, char * argv[])
{
  
  rclcpp::init(argc, argv);
  auto node = std::make_shared<LaserMerger>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
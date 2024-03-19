#include "smrr_cliff_detector/cliff_detector.hpp"

using namespace std;

int main(int argc, char * argv[])
{
  
  rclcpp::init(argc, argv);
  auto node = std::make_shared<CliffDetector>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  
  return 0;
}

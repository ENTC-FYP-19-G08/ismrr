#include "smrr_zed_scanner/zed_scanner.hpp"

using namespace std;

int main(int argc, char * argv[])
{
  
  rclcpp::init(argc, argv);
  auto node = std::make_shared<ZEDScanner>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  
  return 0;
}

#include "imra_diff_drive_controller/controller.hpp"

using namespace std;

int main(int argc, char * argv[])
{
  
  rclcpp::init(argc, argv);
  auto node = std::make_shared<DiffDriveController>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  
  return 0;
}

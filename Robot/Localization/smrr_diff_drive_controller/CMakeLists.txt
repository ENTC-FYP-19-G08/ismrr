cmake_minimum_required(VERSION 3.8)
project(smrr_diff_drive_controller)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(visualization_msgs REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(nav2_msgs REQUIRED)

set(
  DEPENDENCIES
  rclcpp
  std_msgs
  nav_msgs
  tf2_ros
  geometry_msgs
  visualization_msgs
  nav2_msgs
  rclcpp_action
)



# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

add_executable(diff_drive_controller src/controller.cpp src/controller_node.cpp)
ament_target_dependencies(diff_drive_controller ${DEPENDENCIES})

target_include_directories(diff_drive_controller PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_features(diff_drive_controller PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17


install(TARGETS diff_drive_controller
DESTINATION lib/${PROJECT_NAME}
)

install(
  DIRECTORY include launch config
  DESTINATION share/${PROJECT_NAME}
)

ament_package()

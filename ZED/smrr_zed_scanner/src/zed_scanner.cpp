#include "smrr_zed_scanner/zed_scanner.hpp"

// using std::placeholders::_1;
typedef std::unique_ptr<sensor_msgs::msg::PointCloud2> point_cloud_ptr;

ZEDScanner::ZEDScanner() : 
    Node("zed_scanner_node")
{   
    this->declare_parameter("point_cloud_topic", "point_cloud");
    point_cloud_topic_ =this->get_parameter("point_cloud_topic").get_parameter_value().get<std::string>();
    this->declare_parameter("camera_z_offset", 0.0);
    camera_z_offset_ =this->get_parameter("camera_z_offset").get_parameter_value().get<float>();
    this->declare_parameter("transform_tolerance", 0.0);
    transform_tolerance_ = this->get_parameter("transform_tolerance").get_parameter_value().get<float>();
    this->declare_parameter("ground_level", 0.0);
    ground_level_ = this->get_parameter("ground_level").get_parameter_value().get<float>();
    this->declare_parameter("angle_min", 0.0);
    angle_min_ = this->get_parameter("angle_min").get_parameter_value().get<float>();
    this->declare_parameter("angle_max",0.0);
    angle_max_ = this->get_parameter("angle_max").get_parameter_value().get<float>();
    this->declare_parameter("angle_increment", 0.0);
    angle_increment_ = this->get_parameter("angle_increment").get_parameter_value().get<float>();
    this->declare_parameter("scan_time", 0.0);
    scan_time_ = this->get_parameter("scan_time").get_parameter_value().get<float>();
    this->declare_parameter("cliff_range_min", 0.0);
    cliff_range_min_ = this->get_parameter("cliff_range_min").get_parameter_value().get<float>();
    this->declare_parameter("cliff_range_max", 0.0);
    cliff_range_max_ = this->get_parameter("cliff_range_max").get_parameter_value().get<float>();
    this->declare_parameter("obstacle_range_min", 0.0);
    obstacle_range_min_ = this->get_parameter("obstacle_range_min").get_parameter_value().get<float>();
    this->declare_parameter("obstacle_range_max", 0.0);
    obstacle_range_max_ = this->get_parameter("obstacle_range_max").get_parameter_value().get<float>();
    this->declare_parameter("obstacle_height_min", 0.0);
    obstacle_height_min_ = this->get_parameter("obstacle_height_min").get_parameter_value().get<float>();
    this->declare_parameter("obstacle_height_max", 0.0);
    obstacle_height_max_ = this->get_parameter("obstacle_height_max").get_parameter_value().get<float>();
    
    this->declare_parameter("use_inf", true);
    use_inf_ = this->get_parameter("use_inf").get_parameter_value().get<bool>();
    this->declare_parameter("activate_cliff_detector", false);
    activate_cliff_detector_ = this->get_parameter("activate_cliff_detector").get_parameter_value().get<bool>();
    this->declare_parameter("inf_epsilon", 0.0);
    inf_epsilon_ = this->get_parameter("inf_epsilon").get_parameter_value().get<float>();
    this->declare_parameter("activate_filter", false);
    activate_filter_ = this->get_parameter("activate_filter").get_parameter_value().get<bool>();    
    
    //Publishers 
    pub_ = this->create_publisher<sensor_msgs::msg::LaserScan>("smrr_zed_scan", rclcpp::SensorDataQoS());
    filtered_pc_pub_ = this->create_publisher<sensor_msgs::msg::PointCloud2>("filtered_pc", 10);
    debug_param();
    ground_level_ = ground_level_- camera_z_offset_;
    obstacle_height_min_ = obstacle_height_min_- camera_z_offset_;
    obstacle_height_max_ = obstacle_height_max_- camera_z_offset_;
    //Subscribers
    point_cloud_subscriber_ = this->create_subscription<sensor_msgs::msg::PointCloud2>(point_cloud_topic_, rclcpp::SensorDataQoS(), std::bind(&ZEDScanner::callback, this,std::placeholders::_1));
    
    tf_buffer_ =std::make_unique<tf2_ros::Buffer>(this->get_clock());
    tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);
    // timer2_ = this->create_wall_timer(std::chrono::milliseconds(int (1000/odom_publish_rate_)), std::bind(&CliffDetector::publish_odom, this));


    
}

void ZEDScanner::callback(const sensor_msgs::msg::PointCloud2::SharedPtr cloud_msg)
{
  point_cloud_ptr cld_global(new sensor_msgs::msg::PointCloud2());
  pcl::PCLPointCloud2::Ptr cloud_pcl(new pcl::PCLPointCloud2());
  pcl::PCLPointCloud2::Ptr cloud_filtered(new pcl::PCLPointCloud2());

  *cld_global = *cloud_msg;

  if (activate_filter_){
    pcl_conversions::toPCL(*cld_global,*cloud_pcl);
    pcl::VoxelGrid<pcl::PCLPointCloud2> sor;
    sor.setInputCloud(cloud_pcl);
    sor.setFilterFieldName("z");
    sor.setFilterLimits(-4, 0.5);
    sor.setLeafSize (0.02f, 0.02f, 0.02f);
    sor.filter(*cloud_filtered);
    pcl_conversions::fromPCL(*cloud_filtered, *cld_global);
    sor.setMinimumPointsNumberPerVoxel(static_cast<unsigned int>(8));
  }
  filtered_pc_pub_->publish(*cld_global); 

  auto scan_msg = sensor_msgs::msg::LaserScan();
  scan_msg.header = cld_global->header;

  scan_msg.angle_min = angle_min_;
  scan_msg.angle_max = angle_max_;
  scan_msg.angle_increment = angle_increment_;
  scan_msg.time_increment = 0.0;
  scan_msg.scan_time = scan_time_;
  scan_msg.range_min = obstacle_range_min_;
  scan_msg.range_max = obstacle_range_max_;

// //   // determine amount of rays to create
  uint32_t ranges_size = std::ceil(
    (scan_msg.angle_max - scan_msg.angle_min) / scan_msg.angle_increment);

//   // determine if laserscan rays with no obstacle data will evaluate to infinity or max_range
  if (use_inf_) {
    scan_msg.ranges.assign(ranges_size, std::numeric_limits<double>::infinity());
  } else {
    scan_msg.ranges.assign(ranges_size, scan_msg.range_max + inf_epsilon_);
  }


  // Iterate through pointcloud
  for (sensor_msgs::PointCloud2ConstIterator<float> iter_x(*cld_global, "x"),
    iter_y(*cld_global, "y"), iter_z(*cld_global, "z");
    iter_x != iter_x.end(); ++iter_x, ++iter_y, ++iter_z)
  {
    if (std::isnan(*iter_x) || std::isnan(*iter_y) || std::isnan(*iter_z)) {
      RCLCPP_DEBUG(
        this->get_logger(),
        "rejected for nan in point(%f, %f, %f)\n",
        *iter_x, *iter_y, *iter_z);
      continue;
    }

    if ( (*iter_z > ground_level_ && *iter_z < obstacle_height_min_) || *iter_z > obstacle_height_max_) {
      RCLCPP_DEBUG(
        this->get_logger(),
        "rejected for invalid height");
      continue;
    }
    double range;
    if (*iter_z < ground_level_){
        //Cliff filter
      if (activate_cliff_detector_){
        range = hypot(*iter_x, *iter_y);
        range = camera_z_offset_*range/abs(*iter_z);
        if (range < cliff_range_min_) {
            RCLCPP_DEBUG(
              this->get_logger(),
              "rejected for range %f below minimum value %f. Point: (%f, %f, %f)",
              range, cliff_range_min_, *iter_x, *iter_y, *iter_z);
            continue;
          }
          if (range > cliff_range_max_) {
            RCLCPP_DEBUG(
              this->get_logger(),
              "rejected for range %f above maximum value %f. Point: (%f, %f, %f)",
              range, cliff_range_max_, *iter_x, *iter_y, *iter_z);
            continue;
          }
      }
    }

    else{
      // Other obstacles filter
      range = hypot(*iter_x, *iter_y);
      if (range < obstacle_range_min_) {
          RCLCPP_DEBUG(
            this->get_logger(),
            "rejected for range %f below minimum value %f. Point: (%f, %f, %f)",
            range, obstacle_range_min_, *iter_x, *iter_y, *iter_z);
          continue;
        }
        if (range > obstacle_range_max_) {
          RCLCPP_DEBUG(
            this->get_logger(),
            "rejected for range %f above maximum value %f. Point: (%f, %f, %f)",
            range, obstacle_range_max_, *iter_x, *iter_y, *iter_z);
          continue;
        }
    }
    double angle = atan2(*iter_y, *iter_x);
    if (angle < scan_msg.angle_min || angle > scan_msg.angle_max) {
      RCLCPP_DEBUG(
        this->get_logger(),
        "rejected for angle %f not in range (%f, %f)\n",
        angle, scan_msg.angle_min, scan_msg.angle_max);
      continue;
    }
    // overwrite range at laserscan ray if new range is smaller
    int index = (angle - scan_msg.angle_min) / scan_msg.angle_increment;
    if (range < scan_msg.ranges[index]) {
      scan_msg.ranges[index] = range;
    }
  }
    pub_->publish(scan_msg);
}


void ZEDScanner::debug_param(){
    RCLCPP_INFO_STREAM(this->get_logger(), "point_cloud_topic: " << point_cloud_topic_); 
    RCLCPP_INFO_STREAM(this->get_logger(), "camera_z_offset: " << camera_z_offset_); 
    RCLCPP_INFO_STREAM(this->get_logger(), "transform_tolerance: " << transform_tolerance_);
    RCLCPP_INFO_STREAM(this->get_logger(), "ground_level: " << ground_level_);
    RCLCPP_INFO_STREAM(this->get_logger(), "angle_min: " << angle_min_);
    RCLCPP_INFO_STREAM(this->get_logger(), "angle_max: " << angle_max_);
    RCLCPP_INFO_STREAM(this->get_logger(), "angle_increment: " << angle_increment_);
    RCLCPP_INFO_STREAM(this->get_logger(), "scan_time: " << scan_time_);
    RCLCPP_INFO_STREAM(this->get_logger(), "cliff_range_min: " << cliff_range_min_);
    RCLCPP_INFO_STREAM(this->get_logger(), "cliff_range_max: " << cliff_range_max_);
    RCLCPP_INFO_STREAM(this->get_logger(), "obstacle_range_min: " << obstacle_range_min_);
    RCLCPP_INFO_STREAM(this->get_logger(), "obstacle_range_max: " << obstacle_range_max_);
    RCLCPP_INFO_STREAM(this->get_logger(), "obstacle_height_min: " << obstacle_height_min_);
    RCLCPP_INFO_STREAM(this->get_logger(), "obstacle_height_max: " << obstacle_height_max_);
    RCLCPP_INFO_STREAM(this->get_logger(), "use_inf: " << use_inf_);
    RCLCPP_INFO_STREAM(this->get_logger(), "inf_epsilon: " << inf_epsilon_);
    RCLCPP_INFO_STREAM(this->get_logger(), "activate_filter: " << activate_filter_);
}

void ZEDScanner::filter_noise(){
  std::vector<float> values = { 1,2,4,5,1,2,6,6,6,6,3,1,2,2,8,2,1,3,4};
  int i = 0;
  int j = 0;
  int THRESHHOLD = 4;
  int RANGE = 5;
  int count = 0;
  int INF = 9;
  while(i<values.size()){
      if (values[i] < RANGE) {
          // count+=1;
          j = i;
          while(j<values.size() && values[j]<RANGE){
              count+=1;
              j=j+1;
          }

          if(count<THRESHHOLD && j>=values.size()){
              for(int x=i;x<=j-1;x++){
                  values[x] = INF;
              }
              i=j;
          }
          
          else if(count<THRESHHOLD && j<values.size() ){
              for(int x=i;x<=j;x++){
                  values[x] = INF;
                  i=j+1;
              }
              i=j+1;
          }
          else{i=j;}
    
        count = 0;   
      }
      else{
          values[i]=INF;
          i+=1; 
      }
      
  for (float value : values) {
      std::cout << value << " ";
  }
  std::cout << std::endl;

  }
  // Print the updated vector
  for (float value : values) {
      std::cout << value << " ";
  }
  std::cout << std::endl;
}


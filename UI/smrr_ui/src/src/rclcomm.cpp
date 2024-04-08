/*
 * @Author: chengyangkj
 * @Date: 2021-10-30 02:08:29
 * @LastEditTime: 2021-12-01 06:18:27
 * @LastEditors: chengyangkj
 * @Description: ros2的通信类 在这个类进行订阅与发布话题
 * @FilePath: /ros2_qt_demo/src/rclcomm.cpp
 * https://github.com/chengyangkj
 */
#include "rclcomm.h"
#include <iostream>
#include <QString>
#include <QDebug>

rclcomm::rclcomm()
{
    int argc = 0;
    char **argv = NULL;
    rclcpp::init(argc, argv);
    node = rclcpp::Node::make_shared("smrr_ui_node");
    _publisher =
        node->create_publisher<std_msgs::msg::String>("ui_publisher", 10);
    _subscription = node->create_subscription<std_msgs::msg::Int32>("ros2_qt_demo_publish", 10, std::bind(&rclcomm::recv_callback, this, std::placeholders::_1));

    pubGuideNavigation = node->create_publisher<std_msgs::msg::String>("/ui/guide_navigation", 10);
    pubGuideVerbal = node->create_publisher<std_msgs::msg::String>("/ui/guide_verbal", 10);

    pubTrigger = node->create_publisher<std_msgs::msg::String>("/trigger", 10);
    pubUnknownUsername = node->create_publisher<std_msgs::msg::String>("/ui/unknown_username", 10);

    subGuideNavigationResult = node->create_subscription<std_msgs::msg::String>("/ui/guide_navigation_result", 10, std::bind(&rclcomm::onGuideNavigationResult_callback, this, std::placeholders::_1));
    subGuideOptions = node->create_subscription<std_msgs::msg::String>("/ui/guide_options", 10, std::bind(&rclcomm::onGuideOptions_callback, this, std::placeholders::_1));
    subUsername = node->create_subscription<std_msgs::msg::String>("/ui/username", 10, std::bind(&rclcomm::onUsername_callback, this, std::placeholders::_1));
    subTrigger = node->create_subscription<std_msgs::msg::String>("/trigger", 10, std::bind(&rclcomm::onTrigger_callback, this, std::placeholders::_1));

    this->start();
}
void rclcomm::run()
{

    rclcpp::spin(node);
    rclcpp::shutdown();
}
void rclcomm::recv_callback(const std_msgs::msg::Int32::SharedPtr msg)
{
    //  RCLCPP_INFO(node->get_logger(), "I heard: '%d'", msg->data);
    emitTopicData("Recive:" + QString::fromStdString(std::to_string(msg->data)));
}

void rclcomm::onGuideNavigationResult_callback(const std_msgs::msg::String::SharedPtr msg)
{
    //  RCLCPP_INFO(node->get_logger(), "I heard: '%d'", msg->data);
    onGuideNavigationResult(QString::fromStdString(msg->data));
}

void rclcomm::onGuideOptions_callback(const std_msgs::msg::String::SharedPtr msg)
{
    //  RCLCPP_INFO(node->get_logger(), "I heard: '%d'", msg->data);
    onGuideOptions(QString::fromStdString(msg->data));
}

void rclcomm::onUsername_callback(const std_msgs::msg::String::SharedPtr msg)
{
    onUsername(QString::fromStdString(msg->data));
}

void rclcomm::onTrigger_callback(const std_msgs::msg::String::SharedPtr msg)
{
    onTrigger(QString::fromStdString(msg->data));
}

void rclcomm::sendTopicData()
{
    // pub_msg.data++;
    // _publisher->publish(pub_msg);
}

void rclcomm::sendRosData(std::string data)
{
    std_msgs::msg::String rosString;
    rosString.data = data;
    _publisher->publish(rosString);
    qDebug() << "sending ros data: " << QString::fromStdString(data);
}
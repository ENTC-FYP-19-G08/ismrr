/*
 * @Author: chengyangkj
 * @Date: 2021-10-30 02:15:28
 * @LastEditTime: 2021-12-01 06:03:51
 * @LastEditors: chengyangkj
 * @Description: ros2的通信类 在这个类进行订阅与发布话题
 * @FilePath: /ros2_qt_demo/include/ros2_qt_demo/rclcomm.h
 * https://github.com/chengyangkj
 */
#ifndef RCLCOMM_H
#define RCLCOMM_H
#include <QObject>
#include <QThread>
#include <iostream>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/int32.hpp>
#include <std_msgs/msg/string.hpp>

typedef rclcpp::Publisher<std_msgs::msg::String>::SharedPtr PubStr;
typedef rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr PubInt;


typedef rclcpp::Subscription<std_msgs::msg::String>::SharedPtr SubStr;
typedef rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr SubInt;

class rclcomm:public QThread
{
    Q_OBJECT
public:
    rclcomm();
    void publish_topic(int count);
    void recv_callback(const std_msgs::msg::Int32::SharedPtr msg);
    void onGuideNavigationResult_callback(const std_msgs::msg::String::SharedPtr msg);
    void onGuideOptions_callback(const std_msgs::msg::String::SharedPtr msg);
    void onUsername_callback(const std_msgs::msg::String::SharedPtr msg);



    PubStr pubGuideNavigation;
    PubStr pubGuideVerbal;
    PubStr pubTrigger;
    PubStr pubUnknownUsername;

    SubStr subGuideNavigationResult;
    SubStr subGuideOptions;
    SubStr subUsername;

protected:
    void run();
private:
    PubStr _publisher;
    SubInt _subscription;    


    std::shared_ptr<rclcpp::Node> node;

signals:
    void emitTopicData(QString);
    void onGuideNavigationResult(QString);
    void onGuideOptions(QString);
    void onUsername(QString);
    

public slots:
    void sendTopicData();
    void sendRosData(std::string data);
};
#endif // RCLCOMM_H

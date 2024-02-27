/*
 * @Author: chengyangkj
 * @Date: 2021-10-30 03:11:50
 * @LastEditTime: 2021-12-01 06:19:25
 * @LastEditors: chengyangkj
 * @Description: 程序的主入口类
 * @FilePath: /ros2_qt_demo/src/mainwindow.cpp
 * https://github.com/chengyangkj
 */
#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);    
    commNode=new rclcomm();
    connect(commNode,SIGNAL(emitTopicData(QString)),this,SLOT(updateTopicInfo(QString)));
    connect(ui->pushButton,&QPushButton::clicked,commNode,&rclcomm::sendTopicData);
}
void MainWindow::updateTopicInfo(QString data){
    ui->label->clear();
    ui->label->setText(data);
}
MainWindow::~MainWindow()
{
    delete ui;
}


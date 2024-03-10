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
#include <QString>
#include <QProcess>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    commNode = new rclcomm();
    connect(commNode, SIGNAL(emitTopicData(QString)), this, SLOT(updateTopicInfo(QString)));
    connect(ui->pushButton, &QPushButton::clicked, commNode, &rclcomm::sendTopicData);
    qDebug() << "aaa run";
}
void MainWindow::updateTopicInfo(QString data)
{
    ui->label->clear();
    ui->label->setText(data);
}

void MainWindow::on_pushButton2_clicked()
{
    QProcess *process = new QProcess(this);

    // Set the absolute path to the Python script
    QString scriptPath = "/home/csa/fyp/UI/smrr_ui/test/long_running_script.py";

    // Set the command to be executed (Python interpreter with the absolute path to the script as argument)
    QString command = "python3";
    QStringList arguments;
    arguments << scriptPath;

    // Start the process with the command
    process->start(command, arguments);

    // Connect process signals to slots
    QObject::connect(process, &QProcess::readyReadStandardOutput, [=]()
                     {
        // Read the standard output of the process
        
        QString output = process->readAllStandardOutput();
        qDebug() << output;
        // Append the output to the label
        ui->label->setText(ui->label->text() + "\n" + output); });

    connect(process, static_cast<void (QProcess::*)(int)>(&QProcess::finished),

            [=](int exitCode)
            {
                process->deleteLater();
                qDebug() << exitCode;
            }

    );

    
}

MainWindow::~MainWindow()
{
    delete ui;
}

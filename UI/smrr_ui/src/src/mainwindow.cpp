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
#include "page.h"
#include "optionswindow.h"
// #include "./ui_optionswindow.h"
#include <QString>
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

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTopicInfo(QString data)
{
    ui->label->clear();
    ui->label->setText(data);
}

void MainWindow::on_pushButton2_clicked()
{
//    qDebug() << "button 2 click";
//    OptionsWindow q;
//    q.setModal(false);
//    q.exec();
//     q.show();
// static int a=0;
// vector<QString> strings={"aa","bb","ccccc","dd","ee","ih","hello"};
// OptionsWindow *p=new OptionsWindow(this,&strings);
// // p->showFullScreen();
// // p->setAttribute(Qt::WA_DeleteOnClose);
// ui->stackedWidget->addWidget(p);
// ui->stackedWidget->setCurrentIndex(ui->stackedWidget->count()-1);
// OptionsWindow *m=new OptionsWindow(this);
// ui->stackedWidget->addWidget(m);


}

void MainWindow::btnOk_clicked(int i){
qDebug()<<QString("mainwindow btn ok")+QString(i);
}

void MainWindow::btnBack_clicked(){
qDebug()<<QString("mainwindow btn back");

}

QWidget *MainWindow::createWindow(Page *page){
    switch (page->windowId)
    {
    case OPTIONS_WINDOW:
        return new OptionsWindow(this,page);
        break;
    
    default:
        break;
    }
}

void MainWindow::createPages(){
//     pages=new Page[1]{
// {"option1",0,{0,1},"rostopic",0}
//     };

// pages.push_back(Page("option1",OPTIONS_WINDOW,{0,1},"rostopic",0));
// pages.push_back(Page(0));
Page *page= new Page(0);

}




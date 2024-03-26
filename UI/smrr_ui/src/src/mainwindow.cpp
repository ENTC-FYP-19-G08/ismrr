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
#include "sceen_options.h"
// #include "./ui_sceen_options.h"
#include <QString>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    generateAllPages();
    // commNode = new rclcomm();
    // connect(commNode, SIGNAL(emitTopicData(QString)), this, SLOT(updateTopicInfo(QString)));
    // connect(ui->pushButton, &QPushButton::clicked, commNode, &rclcomm::sendTopicData);
    qDebug() << "aaa run";
}

MainWindow::~MainWindow()
{
    delete ui;
    delete pages;
}

void MainWindow::updateTopicInfo(QString data)
{
    ui->label->clear();
    ui->label->setText(data);
}

void MainWindow::on_pushButton2_clicked()
{
    //    qDebug() << "button 2 click";
    //    SceenOptions q;
    //    q.setModal(false);
    //    q.exec();
    //     q.show();
    // static int a=0;
    // vector<QString> strings={"aa","bb","ccccc","dd","ee","ih","hello"};
    // SceenOptions *p=new SceenOptions(this,&strings);
    // // p->showFullScreen();
    // // p->setAttribute(Qt::WA_DeleteOnClose);
    // ui->stackedWidget->addWidget(p);
    // ui->stackedWidget->setCurrentIndex(ui->stackedWidget->count()-1);
    // SceenOptions *m=new SceenOptions(this);
    // ui->stackedWidget->addWidget(m);
    QWidget *screen= createScreen(&pages->at(0));
    ui->stackedWidget->addWidget(screen);


}

void MainWindow::btnOk_clicked(int i)
{
    qDebug() << QString("mainwindow btn ok") + QString(i);
}

void MainWindow::btnBack_clicked()
{
    qDebug() << QString("mainwindow btn back");
}

QWidget *MainWindow::createScreen(Page *page)
{
    switch (page->screenId)
    {
    case WINDOW1:
        return new SceenOptions(this, page);
        break;

    default:
        break;
    }
    return nullptr;
}

void MainWindow::generateAllPages()
{
    //     pages=new Page[1]{
    // {"option1",0,{0,1},"rostopic",0}
    //     };

    // pages.push_back(Page("option1",WINDOW_OPTIONS,{0,1},"rostopic",0));
    // pages.push_back(Page(0));
    // Page *page= new Page(0);
    pages = new vector<Page>;
    pages->push_back(Page("page1",WINDOW1,{PAGE2,PAGE3},"rostopic",0));
    pages->push_back(Page("page2",WINDOW1,{PAGE3,PAGE4,PAGE5},"rostopic",0));
    pages->push_back(Page("page3",WINDOW1,{PAGE6,PAGE1},"rostopic",0));
    pages->push_back(Page("page4",WINDOW1,{PAGE1,PAGE2},"rostopic",0));
    pages->push_back(Page("page5",WINDOW1,{PAGE2,PAGE3},"rostopic",0));
    pages->push_back(Page("page6",WINDOW1,{PAGE5,PAGE1},"rostopic",0));

}

vector<Page> *MainWindow::pages = nullptr;

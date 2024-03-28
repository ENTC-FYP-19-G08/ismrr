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
#include "screen_options.h"
#include "screen_action.h"
// #include "./ui_screen_options.h"
#include <QString>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    generateAllPages();
    btnNext_clicked(PAGE_HOME);
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
    qDebug() << data;
    // ui->label->clear();
    // ui->label->setText(data);
}

// void MainWindow::on_pushButton2_clicked()
// {
//     //    qDebug() << "button 2 click";
//     //    ScreenOptions q;
//     //    q.setModal(false);
//     //    q.exec();
//     //     q.show();
//     // static int a=0;
//     // vector<QString> strings={"aa","bb","ccccc","dd","ee","ih","hello"};
//     // ScreenOptions *p=new ScreenOptions(this,&strings);
//     // // p->showFullScreen();
//     // // p->setAttribute(Qt::WA_DeleteOnClose);
//     // ui->stackedWidget->addWidget(p);
//     // ui->stackedWidget->setCurrentIndex(ui->stackedWidget->count()-1);
//     // ScreenOptions *m=new ScreenOptions(this);
//     // ui->stackedWidget->addWidget(m);
// }

void MainWindow::btnNext_clicked(PAGE_ID nextPageId)
{
    qDebug() << QString("mainwindow btn ok") + QString(nextPageId);
    screen = createScreen(&pages->at(nextPageId));
    ui->stackedWidget->addWidget(screen);
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->count() - 1);
}

void MainWindow::btnBack_clicked()
{

    qDebug() << QString("mainwindow btn back");

    int topScreenIndex = ui->stackedWidget->count() - 1;
    if (topScreenIndex > 0 && screen != nullptr)
    {
        ui->stackedWidget->removeWidget(screen);
        delete screen;
        screen = ui->stackedWidget->widget(topScreenIndex - 1);
    }
}

void MainWindow::btnHome_clicked()
{
    qDebug() << QString("mainwindow btn home");
    int topScreenIndex = ui->stackedWidget->count() - 1;
    while (topScreenIndex > 0)
    {
        screen = ui->stackedWidget->widget(topScreenIndex--);
        ui->stackedWidget->removeWidget(screen);
        delete screen;
    }
}

QWidget *MainWindow::createScreen(Page *page)
{
    switch (page->screenId)
    {
    case SCREEN_OPTIONS:
        return new ScreenOptions(this, page);
        break;
    case SCREEN_ACTION:
        return new ScreenAction(this, page);
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

    // pages.push_back(Page("option1",SCREEN_OPTIONS,{0,1},"rostopic",0));
    // pages.push_back(Page(0));
    // Page *page= new Page(0);
    pages = new vector<Page>;
    // pages->push_back(Page("pageHome", WINDOW1, {PAGE2}, NULL, 0));
    // pages->push_back(Page("page2", WINDOW1, {PAGE3, PAGE4, PAGE5}, "rostopic", 0));
    // pages->push_back(Page("page3", WINDOW1, {PAGE6, PAGE1}, "rostopic", 0));
    // pages->push_back(Page("page4", WINDOW1, {PAGE1, PAGE2}, "rostopic", 0));
    // pages->push_back(Page("page5", WINDOW1, {PAGE2, PAGE3}, "rostopic", 0));
    // pages->push_back(Page("page6", WINDOW1, {PAGE5, PAGE1}, "rostopic", 0));

    pages->push_back(Page("Home", SCREEN_OPTIONS, {PAGE_BASIC_OPTIONS}));
    pages->push_back(Page("Let's Talk", SCREEN_OPTIONS, {PAGE_GUIDE, PAGE_MEET, PAGE_ABOUT_DEPARTMENT}));
    pages->push_back(Page("Guide Me", SCREEN_OPTIONS, {PAGE_LABS}));
    pages->push_back(Page("Meet Someone", SCREEN_OPTIONS, {PAGE_BASIC_OPTIONS}));
    pages->push_back(Page("About Department", SCREEN_OPTIONS, {PAGE_BASIC_OPTIONS}));
    pages->push_back(Page("Labs", SCREEN_OPTIONS, {PAGE_LABS_ANALOG, PAGE_LABS_DIGITAL}));
    pages->push_back(Page("Analog Lab", SCREEN_OPTIONS, {PAGE_BASIC_OPTIONS}));
    pages->push_back(Page("Digital Lab", SCREEN_OPTIONS, {PAGE_BASIC_OPTIONS}));
}

vector<Page> *MainWindow::pages = nullptr;

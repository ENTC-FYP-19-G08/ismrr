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
#include "screen_optionstitled.h"
#include "screen_navigation.h"
#include "screen_map.h"
#include "screen_info.h"
#include "screen_face.h"
#include "screen_name.h"
#include "screen_greet.h"
#include "screen_home.h"

// #include "screen_action.h"

#include <QString>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << "ui run";    

    rosNode = new rclcomm();

    generateLocationMap();    

    gotoPage(PAGE_HOME);
    // gotoPage(PAGE_GUIDE);

    // // connect(rosNode, SIGNAL(emitTopicData(QString)), this, SLOT(updateTopicInfo(QString)));
    // // connect(ui->pushButton, &QPushButton::clicked, rosNode, &rclcomm::sendTopicData);

    
    connect(rosNode, &rclcomm::onGuideOptions, this, &MainWindow::onGuideOptions);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete rosNode;
}

void MainWindow::updateTopicInfo(QString data)
{
    qDebug() << data;
    // ui->label->clear();
    // ui->label->setText(data);
}

void MainWindow::onGuideNavigationResult(QString qdata)
{
    gotoPage(PAGE_INFO, "Navigation Result", qdata.toStdString());
    qDebug() << qdata << "onnavigationinfo main";
}
void MainWindow::onGuideOptions(QString qdata)
{
    string data = qdata.toStdString();
    gotoPage(PAGE_GUIDE_OPTIONS, locationMap[data], data);
    qDebug() << qdata << "onguideoptions main";
}

void MainWindow::gotoPage(PageId pageId, QString text, string data, PubStr pubStr)
{

    switch (pageId)
    {
    case PAGE_HOME:
    {
        vector<Option> options = {Option(PAGE_FACE, "Let's Talk")};
        QWidget *screen = new ScreenHome(this, &options, "Hi!!! \n I'm Devi");
        showScreen(screen);
        break;
    }
    case PAGE_FACE:
    {
        QWidget *screen = new ScreenFace(this);
        showScreen(screen, false);
        break;
    }
    case PAGE_NAME:{
        QWidget *screen = new ScreenName(this);
        showScreen(screen, false);
        break;
    }
    case PAGE_GREET:{
        QWidget *screen = new ScreenGreet(this,"Hi "+text+"! \n How can I help you today ?");
        showScreen(screen, false);
        break;
    }
    case PAGE_GUIDE:
    {
        vector<Option> options = {Option(PAGE_GUIDE_LABS, "Labs"), Option(PAGE_GUIDE_HALLS, "Halls")};
        QWidget *screen = new ScreenOptions(this, &options);
        showScreen(screen);
        break;
    }
    case PAGE_GUIDE_LABS:
    {
        vector<Option> options = {Option(PAGE_GUIDE_OPTIONS, "Vision Lab", "VISION_LAB"), Option(PAGE_GUIDE_OPTIONS, "Telecom Lab", "TELECOM_LAB")};
        QWidget *screen = new ScreenOptions(this, &options);
        showScreen(screen);
        break;
    }
    case PAGE_GUIDE_OPTIONS:
    {
        vector<Option> options = {Option(PAGE_NAVIGATION, "Guide Me", data, rosNode->pubGuideNavigation), Option(PAGE_MAP, "Verbal Instruction", data, rosNode->pubGuideVerbal)};
        QWidget *screen = new ScreenOptionsTitled(this, &options, "Do you want to go to " + text + "?");
        showScreen(screen);
        break;
    }
    case PAGE_NAVIGATION:
    {
        QWidget *screen = new ScreenNavigation(this, "Let' go to " + locationMap[data] + " !!!", data);
        showScreen(screen,false);
        break;
    }
    case PAGE_MAP:
    {
        QWidget *screen = new ScreenMap(this, "Let' go to " + locationMap[data] + ". Map will be displayed here", data);
        showScreen(screen);
        break;
    }
    case PAGE_INFO:
    {
        QWidget *screen = new ScreenInfo(this, text, data);
        showScreen(screen, false);
    }
    default:
        break;
    }

    QString pub = "null";
    if (pubStr == rosNode->pubGuideNavigation)
        pub = "nav";
    else if (pubStr == rosNode->pubGuideVerbal)
        pub = "ver";
    qDebug() << "pageId:" << pageId << " text:" << text << " data:" << QString::fromStdString(data) << " pubStr:" << pub;
}

void MainWindow::showScreen(QWidget *screen, bool screenHist)
{
    int topScreenIndex = ui->stackedWidget->count() - 1;
    if (topScreenIndex > 0 && currentScreen != nullptr && currentScreenHist == false)
    {
        ui->stackedWidget->removeWidget(currentScreen);
        delete currentScreen;
    }

    ui->stackedWidget->addWidget(screen);
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->count() - 1);

    currentScreen = screen;
    currentScreenHist = screenHist;
}

void MainWindow::btnBack_clicked()
{

    qDebug() << QString("mainwindow btn back");

    int topScreenIndex = ui->stackedWidget->count() - 1;
    if (topScreenIndex > 0 && currentScreen != nullptr)
    {
        ui->stackedWidget->removeWidget(currentScreen);
        delete currentScreen;
        currentScreen = ui->stackedWidget->widget(topScreenIndex - 1);
    }
}

void MainWindow::btnHome_clicked()
{
    qDebug() << QString("mainwindow btn home");
    int topScreenIndex = ui->stackedWidget->count() - 1;
    while (topScreenIndex > 0)
    {
        currentScreen = ui->stackedWidget->widget(topScreenIndex--);
        ui->stackedWidget->removeWidget(currentScreen);
        delete currentScreen;
    }
}

QWidget *MainWindow::createScreen(Page *page)
{

    return nullptr;
}

void MainWindow::generateLocationMap()
{
    locationMap["VISION_LAB"] = "Vision Lab";
    locationMap["LIFT"] = "Lift";
    locationMap["PG_ROOM"] = "PG Semminar Room";
    locationMap["TELECOM_LAB"] = "Telecom Lab";
    locationMap["PG_LAB"] = "PG Lab";
    locationMap["3.5_LECTURE_HALL"] = "3.5 Lecture Hall";
    locationMap["WASHROOM"] = "Washrooms";
}

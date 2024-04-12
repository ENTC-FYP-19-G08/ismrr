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
#include "screen_home.h"
#include "screen_name.h"
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
    // gotoPage(PAGE_GUIDE_OPTIONS);

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
        QWidget *screen = new ScreenHome(this);
        showScreen(screen);
        break;
    }
    case PAGE_FACE: // let me remind your name
    {
        QWidget *screen = new ScreenFace(this);
        showScreen(screen, false);
        break;
    }
    case PAGE_NAME:
    {
        QWidget *screen = new ScreenName(this);
        showScreen(screen, false);
        break;
    }
    case PAGE_BASIC_OPTIONS:
    {
        vector<Option> options = {Option(PAGE_GUIDE, "Guide\nMe"), Option(PAGE_MEET, "Meet\nSomeone"), Option(PAGE_ABOUT_DEPARTMENT, "About\nDepartment")};
        QWidget *screen = new ScreenOptionsTitled(this, &options, "Hi " + text + "! \n How can I assist you today?");
        showScreen(screen);
        break;
    }
    case PAGE_GUIDE:
    {
        vector<Option> options = {Option(PAGE_GUIDE_LABS, "Labs"), Option(PAGE_GUIDE_HALLS, "Halls"), Option(PAGE_GUIDE_OTHER, "Other")};
        QWidget *screen = new ScreenOptions(this, &options);
        showScreen(screen);
        break;
    }
    case PAGE_GUIDE_LABS:
    {
        vector<Option> options = {Option(PAGE_GUIDE_OPTIONS, locationMap["VISION_LAB"], "VISION_LAB"), Option(PAGE_GUIDE_OPTIONS, locationMap["TELECOM_LAB"], "TELECOM_LAB")};
        QWidget *screen = new ScreenOptions(this, &options);
        showScreen(screen);
        break;
    }
    case PAGE_GUIDE_HALLS:
    {
        vector<Option> options = {Option(PAGE_GUIDE_OPTIONS, locationMap["PG_ROOM"], "PG_ROOM"), Option(PAGE_GUIDE_OPTIONS, locationMap["3.5_LECTURE_HALL"], "3.5_LECTURE_HALL")};
        QWidget *screen = new ScreenOptions(this, &options);
        showScreen(screen);
        break;
    }
    case PAGE_GUIDE_OTHER:
    {
        vector<Option> options = {Option(PAGE_GUIDE_OPTIONS, locationMap["WASHROOM"], "WASHROOM"), Option(PAGE_GUIDE_OPTIONS, locationMap["LIFT"], "LIFT")};
        QWidget *screen = new ScreenOptions(this, &options);
        showScreen(screen);
        break;
    }
    case PAGE_GUIDE_OPTIONS:
    {
        vector<Option> options = {Option(PAGE_NAVIGATION, "Guide\nMe", data, rosNode->pubGuideNavigation), Option(PAGE_MAP, "Verbal\nInstruction", data, rosNode->pubGuideVerbal)};
        QWidget *screen = new ScreenOptionsTitled(this, &options, "Do you want to go to " + text + "?");
        showScreen(screen);
        break;
    }
    case PAGE_NAVIGATION:
    {
        QWidget *screen = new ScreenNavigation(this, "Let' go to " + locationMap[data] + " !!!", data);
        showScreen(screen, false);
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
        break;
    }
    case PAGE_SETTINGS:
    {
        vector<Option> options = {Option(PAGE_ACTION_MINIMIZE, "Minimize"),Option(PAGE_ACTION_FULLSCREEN, "Full Screen"), Option(PAGE_ACTION_RES_640x480, "640x480"), Option(PAGE_ACTION_RES_1280x720, "1280x720")};
        QWidget *screen = new ScreenOptions(this, &options);
        showScreen(screen);
        break;
    }
    case PAGE_ACTION_MINIMIZE:
    {
        this->setWindowState(Qt::WindowMinimized);
        break;
    }
    case PAGE_ACTION_FULLSCREEN:
    {
        this->setWindowState(Qt::WindowFullScreen);
        break;
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

// void MainWindow::publishStr(PubStr pubStr, QString qdata)
// {
//     std_msgs::msg::String rosString;
//     rosString.data = qdata.toStdString();
//     pubStr->publish(rosString);
// }
void MainWindow::publishStr(PubStr pubStr, string data)
{
    std_msgs::msg::String rosString;
    rosString.data = data;
    pubStr->publish(rosString);
}
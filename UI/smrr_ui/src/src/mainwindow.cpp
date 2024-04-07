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

// #include "screen_action.h"

#include <QString>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << "aaa run";
    // PAGE_ID pgid = (PAGE_ID)100000;
    // qDebug() << pgid;

    rosNode = new rclcomm();

    generateLocationMap();
    qDebug() << locationMap["ANALOG_LAB"];

    gotoPage(PAGE_HOME);
    // gotoPage(PAGE_GUIDE);

    // // connect(rosNode, SIGNAL(emitTopicData(QString)), this, SLOT(updateTopicInfo(QString)));
    // // connect(ui->pushButton, &QPushButton::clicked, rosNode, &rclcomm::sendTopicData);

    // connect(rosNode, &rclcomm::onGuideNavigationResult, this, &MainWindow::onGuideNavigationResult);
    connect(rosNode, &rclcomm::onGuideOptions, this, &MainWindow::onGuideOptions);

    // qDebug() << "aaa run";
}

MainWindow::~MainWindow()
{
    delete ui;

    for (auto p : *pages)
    {
        delete p;
    }
    pages->clear();

    delete pages;
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
        QWidget *screen = new ScreenOptionsTitled(this, &options, "Hi!!! \n I'm Devi");
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
    // int topScreenIndex = ui->stackedWidget->count() - 1;
    // if (topScreenIndex > 0 && currentScreen != nullptr && currentScreenHist == false)
    // {
    //     ui->stackedWidget->removeWidget(currentScreen);
    //     delete currentScreen;
    // }

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

void MainWindow::generateAllPages()
{
    // //     pages=new Page[1]{
    // // {"option1",0,{0,1},"rostopic",0}
    // //     };

    // // pages.push_back(Page("option1",SCREEN_MENU_OPTIONS,{0,1},"rostopic",0));
    // // pages.push_back(Page(0));    // // Page *page= new Page(0);

    // pages = new vector<Page *>;
    // // pages->push_back(Page("pageHome", WINDOW1, {PAGE2}, NULL, 0));
    // // pages->push_back(Page("page2", WINDOW1, {PAGE3, PAGE4, PAGE5}, "rostopic", 0));
    // // pages->push_back(Page("page3", WINDOW1, {PAGE6, PAGE1}, "rostopic", 0));
    // // pages->push_back(Page("page4", WINDOW1, {PAGE1, PAGE2}, "rostopic", 0));
    // // pages->push_back(Page("page5", WINDOW1, {PAGE2, PAGE3}, "rostopic", 0));
    // // pages->push_back(Page("page6", WINDOW1, {PAGE5, PAGE1}, "rostopic", 0));

    // // /*PAGE_HOME*/ pages->push_back(Page("Home", SCREEN_MENU_OPTIONS, {PAGE_BASIC_OPTIONS}));
    // // /*PAGE_BASIC_OPTIONS*/ pages->push_back(Page("Let's Talk", SCREEN_MENU_OPTIONS, {PAGE_GUIDE}));
    // // /*  PAGE_GUIDE*/ pages->push_back(Page("Guide Me", SCREEN_MENU_OPTIONS, {PAGE_GUIDE_OFFICE}));
    // // /*      PAGE_GUIDE_OFFICE*/ pages->push_back(Page("Office", SCREEN_GUIDE_OPTIONS, {PAGE_HOME, PAGE_HOME}, rosNode->pubGuideNavigation, "OFFICE"));

    // /*PAGE_HOME*/ pages->push_back(new Page("Home", SCREEN_MENU_OPTIONS, {PAGE_BASIC_OPTIONS}));
    // /*PAGE_BASIC_OPTIONS*/ pages->push_back(new Page("Let's Talk", SCREEN_MENU_OPTIONS, {PAGE_GUIDE}));
    // /*  PAGE_GUIDE*/ pages->push_back(new Page("Guide Me", SCREEN_MENU_OPTIONS, {PAGE_GUIDE_OFFICE}));
    // /*      PAGE_GUIDE_OFFICE*/ pages->push_back(new Page("Office", SCREEN_GUIDE_OPTIONS, {PAGE_HOME, PAGE_HOME}, rosNode->pubGuideNavigation, "OFFICE"));

    // // /*PAGE_HOME*/ pages->push_back(Page("Home", SCREEN_MENU_OPTIONS, {PAGE_BASIC_OPTIONS}));
    // // /*PAGE_BASIC_OPTIONS*/ pages->push_back(Page("Let's Talk", SCREEN_MENU_OPTIONS, {PAGE_GUIDE, PAGE_MEET, PAGE_ABOUT_DEPARTMENT}));
    // // /*  PAGE_GUIDE*/ pages->push_back(Page("Guide Me", SCREEN_MENU_OPTIONS, {PAGE_GUIDE_OFFICE,PAGE_GUIDE_LABS, PAGE_GUIDE_HALLS,PAGE_GUIDE_OTHER}));
    // // /*      PAGE_GUIDE_OFFICE*/ pages->push_back(Page("Office", SCREEN_ACTION, {},rosNode->pubGuideNavigation,"OFFICE"));
    // // /*      PAGE_GUIDE_LABS*/ pages->push_back(Page("Labs", SCREEN_MENU_OPTIONS, {PAGE_GUIDE_LAB_ANALOG,PAGE_GUIDE_LAB_DIGITAL,PAGE_GUIDE_LAB_TELECOM,PAGE_GUIDE_LAB_BM}));
    // // /*          PAGE_GUIDE_LAB_ANALOG*/ pages->push_back(Page("Analog Lab", SCREEN_ACTION, {},"navigation/LAB_ANALOG"));
    // // /*          PAGE_GUIDE_LAB_DIGITAL*/ pages->push_back(Page("Digital Lab", SCREEN_ACTION, {},"navigation/LAB_DIGITAL"));
    // // /*          PAGE_GUIDE_LAB_TELECOM*/ pages->push_back(Page("Telecom Lab", SCREEN_ACTION, {},"navigation/LAB_TELECOM"));
    // // /*          PAGE_GUIDE_LAB_BM*/ pages->push_back(Page("Bio-Medical Lab", SCREEN_ACTION, {},"navigation/LAB_BM"));
    // // /*      PAGE_GUIDE_HALLS*/ pages->push_back(Page("Halls", SCREEN_MENU_OPTIONS, {PAGE_GUIDE_HALL_ENTC1,PAGE_GUIDE_HALL_PG,PAGE_GUIDE_HALL_3P5}));
    // // /*          PAGE_GUIDE_HALL_ENTC1*/ pages->push_back(Page("ENTC1", SCREEN_ACTION, {},"navigation/HALL_ENTC1"));
    // // /*          PAGE_GUIDE_HALL_PG*/ pages->push_back(Page("PG Seminar Room", SCREEN_ACTION, {},"navigation/HALL_PG"));
    // // /*          PAGE_GUIDE_HALL_3P5*/ pages->push_back(Page("3.5 Hall", SCREEN_ACTION, {},"navigation/HALL_3P5"));
    // // /*      PAGE_GUIDE_OTHER*/ pages->push_back(Page("Other", SCREEN_MENU_OPTIONS, {PAGE_GUIDE_OTHER_WASHROOMS,PAGE_GUIDE_OTHER_LIFT,PAGE_GUIDE_OTHER_COMMON_LOWER,PAGE_GUIDE_OTHER_COMMON_UPPER}));
    // // /*          PAGE_GUIDE_OTHER_WASHROOMS*/ pages->push_back(Page("Washrooms", SCREEN_ACTION, {},"navigation/WASHROOMS"));
    // // /*          PAGE_GUIDE_OTHER_LIFT*/ pages->push_back(Page("Lift", SCREEN_ACTION, {},"navigation/LIFT"));
    // // /*          PAGE_GUIDE_OTHER_COMMON_LOWER*/ pages->push_back(Page("Lower Common", SCREEN_ACTION, {},"navigation/COMMON_LOWER"));
    // // /*          PAGE_GUIDE_OTHER_COMMON_UPPER*/ pages->push_back(Page("Upper Common", SCREEN_ACTION, {},"navigation/COMMON_UPPER"));

    // // /*  PAGE_MEET*/ pages->push_back(Page("Meet Someone", SCREEN_MENU_OPTIONS, {PAGE_BASIC_OPTIONS}));
    // // /*  PAGE_ABOUT_DEPARTMENT*/ pages->push_back(Page("About Department", SCREEN_MENU_OPTIONS, {PAGE_BASIC_OPTIONS}));
}

// vector<Page> *MainWindow::pages = nullptr;


#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "screen_options.h"
#include "screen_optionstitled.h"
#include "screen_navigation.h"
#include "screen_verbal.h"
#include "screen_info.h"
#include "screen_face.h"
#include "screen_home.h"
#include "screen_name.h"

#include <QString>
#include <QDebug>
#include <cstdlib>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{

    border = new WidgetBlinker(this, new WidgetBorder(this));

    ui->setupUi(this);

    listenToggler = new WidgetToggler(this, ui->btnListen, RES_PATH "listen.png", RES_PATH "not_listen.png");
    listenToggler->setChecked(true);
    
    qDebug() << "ui run";

    rosNode = new rclcomm();

    generateLocationData();

    gotoPage(PAGE_HOME);
    // gotoPage(PAGE_GUIDE);
    // gotoPage(PAGE_GUIDE_OPTIONS);

    connect(rosNode, &rclcomm::onGuideOptions, this, &MainWindow::onGuideOptions);
    connect(rosNode, &rclcomm::onChangeState, this, &MainWindow::onChangeState);

    connect(listenToggler, &WidgetToggler::toggled, this, &MainWindow::listenToggler_toggled);
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
    border->blink(5, 300);
    qDebug() << qdata << "onguideoptions main";
}

void MainWindow::onChangeState(QString qdata)
{
    string data = qdata.toStdString();

    if (data == "IDLE")
        btnHome_clicked();
    else if (data == "LISTEN_START")
        listenToggler->setChecked(true);
    else if (data == "LISTEN_STOP")
        listenToggler->setChecked(false);
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
        vector<Option> options = {Option(PAGE_GUIDE, "Guide\nMe"), Option(PAGE_MEET, "Meet\nSomeone"), Option(PAGE_ABOUT, "About\nDevelopers")};
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
        vector<Option> options;
        loadOptionsFromPrefix(&options, "LAB_");
        QWidget *screen = new ScreenOptions(this, &options);
        showScreen(screen);
        break;
    }
    case PAGE_GUIDE_HALLS:
    {
        vector<Option> options;
        loadOptionsFromPrefix(&options, "HALL_");
        QWidget *screen = new ScreenOptions(this, &options);
        showScreen(screen);
        break;
    }
    case PAGE_GUIDE_OTHER:
    {
        vector<Option> options = {Option(PAGE_GUIDE_OPTIONS, locationMap["WASHROOMS_COMMON"], "WASHROOMS_COMMON"), Option(PAGE_GUIDE_OPTIONS, locationMap["LIFT"], "LIFT")};
        QWidget *screen = new ScreenOptions(this, &options);
        showScreen(screen);
        break;
    }
    case PAGE_GUIDE_OPTIONS:
    {
        vector<Option> options = {Option(PAGE_VERBAL, "Verbal\nInstruction", data, rosNode->pubGuideVerbal)};
        if (reachableLocations.find(data) != reachableLocations.end())
            options.push_back(Option(PAGE_NAVIGATION, "Guide\nMe", data, rosNode->pubGuideNavigation));

        if (data.find("PERSON_") == 0)
            text = "Do you want to meet " + text + "?";
        else
            text = "Do you want to go to " + text + "?";

        QWidget *screen = new ScreenOptionsTitled(this, &options, text);
        showScreen(screen);
        break;
    }
    case PAGE_NAVIGATION:
    {
        QWidget *screen = new ScreenNavigation(this, "Let' go to " + locationMap[data] + " !!!", data);
        showScreen(screen, false);
        break;
    }
    case PAGE_VERBAL:
    {
        // QWidget *screen = new ScreenMap(this, "Let' go to " + locationMap[data] + ". Map will be displayed here", data);
        QWidget *screen = new ScreenVerbal(this, locationMap[data], data);
        showScreen(screen);
        break;
    }
    case PAGE_MEET:
    {
        vector<Option> options = {Option(PAGE_GUIDE_OPTIONS, locationMap["PERSON_RANGA"], "PERSON_RANGA"), Option(PAGE_GUIDE_OPTIONS, locationMap["PERSON_PESHALA"], "PERSON_PESHALA")};
        QWidget *screen = new ScreenOptions(this, &options);
        showScreen(screen);
        break;
    }
    case PAGE_ABOUT:
    {
        gotoPage(PAGE_VERBAL, "About Developers", "ABOUT");
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
        vector<Option> options = {Option(PAGE_ACTION_MINIMIZE, "Minimize"), Option(PAGE_ACTION_FULLSCREEN, "Full Screen"), Option(PAGE_ACTION_RES_640x480, "640x480"), Option(PAGE_ACTION_RES_1280x720, "1280x720")};
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
    case PAGE_ACTION_RES_640x480:
    {
        system("xrandr --output eDP-1 --mode 640x480");
        system("xrandr --output HDMI-0 --mode 640x480");
        break;
    }
    case PAGE_ACTION_RES_1280x720:
    {
        system("xrandr --output eDP-1 --mode 1920x1080");
        system("xrandr --output HDMI-0 --mode 1280x720");
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

void MainWindow::listenToggler_toggled(bool checked)
{
    if (checked)
        publishStr(rosNode->pubListenState, "START");
    else
        publishStr(rosNode->pubListenState, "STOP");
    qDebug() << "Listen toggler" << checked;
}

QWidget *MainWindow::createScreen(Page *page)
{

    return nullptr;
}

void MainWindow::generateLocationData()
{
    locationMap["HALL_ENTC1"] = "ENTC1";
    locationMap["COMMON_LOWER"] = "Lower Common";
    locationMap["LAB_BM"] = "BM Lab";
    locationMap["LAB_UAV"] = "UAV LAB";
    locationMap["LIFT"] = "Lift";
    locationMap["WASHROOMS_COMMON"] = "Washrooms";
    locationMap["WASHROOMS_STAFF"] = "Staff Washrooms";
    locationMap["LAB_COMPUTER"] = "Computer Lab";
    locationMap["ROOM_LECTURERS"] = "Lecturer Rooms";
    locationMap["ROOM_STAFF"] = "Staff Room";
    locationMap["OFFICE"] = "Department Office";
    locationMap["ROOM_CONFERENCE"] = "Conference Room";
    locationMap["ROOM_HOD"] = "HOD office";
    locationMap["LAB_ANALOG"] = "Analog Lab";
    locationMap["COMMON_UPPER"] = "Upper Common";
    locationMap["WORKSHOP"] = "Workshop";
    locationMap["ROOM_SOLDER"] = "Soldering Room";
    locationMap["LAB_DIGITAL"] = "Digital Lab";
    locationMap["HALL_PG"] = "PG Seminar Room";
    locationMap["LAB_DIALOG"] = "Dialog Research Lab";
    locationMap["LAB_TELECOM"] = "Telecom Lab";
    locationMap["LAB_VISION"] = "Vision Lab";
    locationMap["LAB_PG"] = "PG Lab";
    locationMap["HALL_3.5"] = "3.5 Hall";
    locationMap["ROOM_INSTRUCTORS"] = "Instructors Room";

    // locationMap["PERSON_PESHALA"] = "Dr. Peshala Jayasekara";
    // locationMap["PERSON_ROHAN"] = "Prof. Rohan Munasinghe";
    // locationMap["PERSON_DILEEKA"] = "Prof. Dileeka Dias";
    // locationMap["PERSON_JAYASINGHE"] = "Prof. J.A.K.S. Jayasinghe";
    // locationMap["PERSON_RANGA"] = "Dr. Ranga Rodrigo";
    // locationMap["PERSON_KITHSIRI"] = "Eng. A.T.L.K. Samarasinghe";
    // locationMap["PERSON_AJITH"] = "Dr. Ajith Pasqual";

    locationMap["PERSON_RANGA"] = "Dr. Ranga Rodrigo";
    locationMap["PERSON_PESHALA"] = "Dr. Peshala Jayasekara";

    locationMap["ABOUT"] = "About Developers";

    /**/

    reachableLocations = {
        "HALL_PG",
        "LAB_DIALOG",
        "LAB_TELECOM",
        "LAB_VISION",
        "LAB_PG",
        "HALL_3.5",
        "LIFT",
        "WASHROOMS_COMMON"};
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

void MainWindow::loadOptionsFromPrefix(vector<Option> *options, string prefix)
{
    for (const auto &pair : locationMap)
    {
        if (pair.first.find(prefix) == 0)
            options->push_back(Option(PAGE_GUIDE_OPTIONS, pair.second, pair.first));
    }
}

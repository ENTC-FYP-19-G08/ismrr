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

    qDebug() << "aaa run";
    // PAGE_ID pgid = (PAGE_ID)100000;
    // qDebug() << pgid;

    rosNode = new rclcomm();
    // // gotoPage(PAGE_LABS);

    generateAllPages();
    // gotoPage(PAGE_HOME);
    gotoPage(PAGE_GUIDE);

    // // connect(rosNode, SIGNAL(emitTopicData(QString)), this, SLOT(updateTopicInfo(QString)));
    // // connect(ui->pushButton, &QPushButton::clicked, rosNode, &rclcomm::sendTopicData);

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

void MainWindow::gotoPage(int pageId)
{
    qDebug() << QString("mainwindow btn ok") + QString(pageId);

    int topScreenIndex = ui->stackedWidget->count() - 1;
    if (topScreenIndex > 0 && currentScreen != nullptr && currentPage !=nullptr && currentPage->noHist)
    {
        ui->stackedWidget->removeWidget(currentScreen);
        delete currentScreen;        
    }


    currentPage=pages->at(pageId);
    currentScreen = createScreen(currentPage);
    ui->stackedWidget->addWidget(currentScreen);
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->count() - 1);
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
    switch (page->screenId)
    {
    case SCREEN_MENU_OPTIONS:
        return new ScreenOptions(this, page);
        break;
    case SCREEN_GUIDE_OPTIONS:

        page->nextPageIds.clear();
        qDebug() << "pages count in SCREEN_OPTION_GUIDE_1" << pages->size();
        page->nextPageIds.push_back(pages->size());
        pages->push_back(new Page("Let's Go", SCREEN_ACTION, {PAGE_HOME}, rosNode->pubNavigation, page->rosData,true));
        page->nextPageIds.push_back(pages->size());
        pages->push_back(new Page("Give Instructions", SCREEN_ACTION, {PAGE_HOME}, rosNode->pubGuideIns, page->rosData,true));

        qDebug() << "pages count in SCREEN_OPTION_GUIDE_2" << pages->size();

        qDebug() << page->nextPageIds[0] << page->name << QString::fromStdString(page->rosData);

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

    // pages.push_back(Page("option1",SCREEN_MENU_OPTIONS,{0,1},"rostopic",0));
    // pages.push_back(Page(0));
    // Page *page= new Page(0);

    pages = new vector<Page *>;
    // pages->push_back(Page("pageHome", WINDOW1, {PAGE2}, NULL, 0));
    // pages->push_back(Page("page2", WINDOW1, {PAGE3, PAGE4, PAGE5}, "rostopic", 0));
    // pages->push_back(Page("page3", WINDOW1, {PAGE6, PAGE1}, "rostopic", 0));
    // pages->push_back(Page("page4", WINDOW1, {PAGE1, PAGE2}, "rostopic", 0));
    // pages->push_back(Page("page5", WINDOW1, {PAGE2, PAGE3}, "rostopic", 0));
    // pages->push_back(Page("page6", WINDOW1, {PAGE5, PAGE1}, "rostopic", 0));

    // /*PAGE_HOME*/ pages->push_back(Page("Home", SCREEN_MENU_OPTIONS, {PAGE_BASIC_OPTIONS}));
    // /*PAGE_BASIC_OPTIONS*/ pages->push_back(Page("Let's Talk", SCREEN_MENU_OPTIONS, {PAGE_GUIDE}));
    // /*  PAGE_GUIDE*/ pages->push_back(Page("Guide Me", SCREEN_MENU_OPTIONS, {PAGE_GUIDE_OFFICE}));
    // /*      PAGE_GUIDE_OFFICE*/ pages->push_back(Page("Office", SCREEN_GUIDE_OPTIONS, {PAGE_HOME, PAGE_HOME}, rosNode->pubNavigation, "OFFICE"));

    /*PAGE_HOME*/ pages->push_back(new Page("Home", SCREEN_MENU_OPTIONS, {PAGE_BASIC_OPTIONS}));
    /*PAGE_BASIC_OPTIONS*/ pages->push_back(new Page("Let's Talk", SCREEN_MENU_OPTIONS, {PAGE_GUIDE}));
    /*  PAGE_GUIDE*/ pages->push_back(new Page("Guide Me", SCREEN_MENU_OPTIONS, {PAGE_GUIDE_OFFICE}));
    /*      PAGE_GUIDE_OFFICE*/ pages->push_back(new Page("Office", SCREEN_GUIDE_OPTIONS, {PAGE_HOME, PAGE_HOME}, rosNode->pubNavigation, "OFFICE"));

    // /*PAGE_HOME*/ pages->push_back(Page("Home", SCREEN_MENU_OPTIONS, {PAGE_BASIC_OPTIONS}));
    // /*PAGE_BASIC_OPTIONS*/ pages->push_back(Page("Let's Talk", SCREEN_MENU_OPTIONS, {PAGE_GUIDE, PAGE_MEET, PAGE_ABOUT_DEPARTMENT}));
    // /*  PAGE_GUIDE*/ pages->push_back(Page("Guide Me", SCREEN_MENU_OPTIONS, {PAGE_GUIDE_OFFICE,PAGE_GUIDE_LABS, PAGE_GUIDE_HALLS,PAGE_GUIDE_OTHER}));
    // /*      PAGE_GUIDE_OFFICE*/ pages->push_back(Page("Office", SCREEN_ACTION, {},rosNode->pubNavigation,"OFFICE"));
    // /*      PAGE_GUIDE_LABS*/ pages->push_back(Page("Labs", SCREEN_MENU_OPTIONS, {PAGE_GUIDE_LAB_ANALOG,PAGE_GUIDE_LAB_DIGITAL,PAGE_GUIDE_LAB_TELECOM,PAGE_GUIDE_LAB_BM}));
    // /*          PAGE_GUIDE_LAB_ANALOG*/ pages->push_back(Page("Analog Lab", SCREEN_ACTION, {},"navigation/LAB_ANALOG"));
    // /*          PAGE_GUIDE_LAB_DIGITAL*/ pages->push_back(Page("Digital Lab", SCREEN_ACTION, {},"navigation/LAB_DIGITAL"));
    // /*          PAGE_GUIDE_LAB_TELECOM*/ pages->push_back(Page("Telecom Lab", SCREEN_ACTION, {},"navigation/LAB_TELECOM"));
    // /*          PAGE_GUIDE_LAB_BM*/ pages->push_back(Page("Bio-Medical Lab", SCREEN_ACTION, {},"navigation/LAB_BM"));
    // /*      PAGE_GUIDE_HALLS*/ pages->push_back(Page("Halls", SCREEN_MENU_OPTIONS, {PAGE_GUIDE_HALL_ENTC1,PAGE_GUIDE_HALL_PG,PAGE_GUIDE_HALL_3P5}));
    // /*          PAGE_GUIDE_HALL_ENTC1*/ pages->push_back(Page("ENTC1", SCREEN_ACTION, {},"navigation/HALL_ENTC1"));
    // /*          PAGE_GUIDE_HALL_PG*/ pages->push_back(Page("PG Seminar Room", SCREEN_ACTION, {},"navigation/HALL_PG"));
    // /*          PAGE_GUIDE_HALL_3P5*/ pages->push_back(Page("3.5 Hall", SCREEN_ACTION, {},"navigation/HALL_3P5"));
    // /*      PAGE_GUIDE_OTHER*/ pages->push_back(Page("Other", SCREEN_MENU_OPTIONS, {PAGE_GUIDE_OTHER_WASHROOMS,PAGE_GUIDE_OTHER_LIFT,PAGE_GUIDE_OTHER_COMMON_LOWER,PAGE_GUIDE_OTHER_COMMON_UPPER}));
    // /*          PAGE_GUIDE_OTHER_WASHROOMS*/ pages->push_back(Page("Washrooms", SCREEN_ACTION, {},"navigation/WASHROOMS"));
    // /*          PAGE_GUIDE_OTHER_LIFT*/ pages->push_back(Page("Lift", SCREEN_ACTION, {},"navigation/LIFT"));
    // /*          PAGE_GUIDE_OTHER_COMMON_LOWER*/ pages->push_back(Page("Lower Common", SCREEN_ACTION, {},"navigation/COMMON_LOWER"));
    // /*          PAGE_GUIDE_OTHER_COMMON_UPPER*/ pages->push_back(Page("Upper Common", SCREEN_ACTION, {},"navigation/COMMON_UPPER"));

    // /*  PAGE_MEET*/ pages->push_back(Page("Meet Someone", SCREEN_MENU_OPTIONS, {PAGE_BASIC_OPTIONS}));
    // /*  PAGE_ABOUT_DEPARTMENT*/ pages->push_back(Page("About Department", SCREEN_MENU_OPTIONS, {PAGE_BASIC_OPTIONS}));
}

// vector<Page> *MainWindow::pages = nullptr;

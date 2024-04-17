
#include "screen_home.h"
#include "./ui_screen_home.h"

#include <QDebug>

ScreenHome::ScreenHome(QWidget *parent)
    : QDialog(parent), ui(new Ui::ScreenHome)
{
    ui->setupUi(this);

    mainWindow = static_cast<MainWindow *>(parent);

    qDebug() << "Home window loaded";

    connect(ui->btnTalk, &QPushButton::clicked, [this]()
            { mainWindow->publishStr(mainWindow->rosNode->pubTrigger, "<TRIG>"); });

    connect(ui->btnSettings, &QPushButton::clicked, [this]()
            { mainWindow->gotoPage(PAGE_SETTINGS); });

    connect(mainWindow->rosNode, &rclcomm::onTrigger, this, &ScreenHome::onTrigger);
}

ScreenHome::~ScreenHome()
{
    delete ui;
}

void ScreenHome::onTrigger(QString qdata)
{
    qDebug() << "Screen Home Trigger:" << qdata;
    // ui->label->setText(qdata);
    if (qdata == "<TRIG>")
        mainWindow->gotoPage(PAGE_FACE);
}
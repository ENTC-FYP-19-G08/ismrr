
#include "screen_home.h"
#include "./ui_screen_home.h"

#include <QDebug>

ScreenHome::ScreenHome(QWidget *parent, vector<Option> *options, QString text)
    : QDialog(parent), ui(new Ui::ScreenHome)
{
    ui->setupUi(this);

    mainWindow = static_cast<MainWindow *>(parent);
    MainWindow *_mainWindow = mainWindow;

    qDebug() << "Home window loaded";

    ui->label->setText(text);

    for (uint i = 0; i < options->size(); i++)
    {
        qDebug() << "screen_options:i" << i;

        Option option = options->at(i);
        QPushButton *btnOption = new QPushButton(option.text, this);

        btnOption->setMinimumHeight(100);
        btnOption->setMaximumHeight(110);

        connect(btnOption, &QPushButton::clicked, [_mainWindow, option]()
                { _mainWindow->gotoPage(option.pageId, option.text, option.data); });
        ui->scrollLayout->addWidget(btnOption);
    }

    connect(mainWindow->rosNode, &rclcomm::onUsername, this, &ScreenHome::onUsername);

    connect(ui->btnBack, &QPushButton::clicked, mainWindow, &MainWindow::btnBack_clicked);
    connect(ui->btnHome, &QPushButton::clicked, mainWindow, &MainWindow::btnHome_clicked);
}

ScreenHome::~ScreenHome()
{
    delete ui;
}

void ScreenHome::onUsername(QString qdata)
{
    qDebug() << "ScreenFace Sub:" << qdata;
    // ui->label->setText(qdata);
    if (qdata == "unknown")
        mainWindow->gotoPage(PAGE_NAME);
    else
        mainWindow->gotoPage(PAGE_GREET, qdata);
}
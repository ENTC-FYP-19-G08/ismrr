
#include "screen_splash.h"
#include "./ui_screen_splash.h"

#include <QDebug>

ScreenSplash::ScreenSplash(QWidget *parent /*TODO: , vector<Option> *options, QString text, PubStr *pubStr, string data */)
    : QDialog(parent), ui(new Ui::ScreenSplash)
{
    ui->setupUi(this);

    MainWindow *mainWindow = static_cast<MainWindow *>(parent);

    qDebug() << "Splash window loaded";

    // ui->label->setText();

    for (uint i = 0; i < options->size(); i++)
    {
        qDebug() << "screen_options:i" << i;

        Option option = options->at(i);
        QPushButton *btnOption = new QPushButton(option.text, this);
        
        btnOption->setMinimumHeight(100);
        btnOption->setMaximumHeight(110);
        
        connect(btnOption, &QPushButton::clicked, [mainWindow, option]()
                { mainWindow->gotoPage(option.pageId, option.text, option.data); });
        ui->scrollLayout->addWidget(btnOption);
       
    }

    connect(ui->btnBack, &QPushButton::clicked, mainWindow, &MainWindow::btnBack_clicked);
    connect(ui->btnHome, &QPushButton::clicked, mainWindow, &MainWindow::btnHome_clicked);

}

ScreenSplash::~ScreenSplash()
{
    delete ui;
}

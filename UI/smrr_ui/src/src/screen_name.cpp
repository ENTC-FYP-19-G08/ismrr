
#include "screen_name.h"
#include "./ui_screen_name.h"

#include <QDebug>

ScreenName::ScreenName(QWidget *parent /*TODO: , vector<Option> *options, QString text, PubStr *pubStr, string data */)
    : QDialog(parent), ui(new Ui::ScreenName)
{
    ui->setupUi(this);

    MainWindow *mainWindow = static_cast<MainWindow *>(parent);

    qDebug() << "Name window loaded";

    // ui->label->setText();

    // for (uint i = 0; i < options->size(); i++)
    // {
    //     qDebug() << "screen_options:i" << i;

    //     Option option = options->at(i);
    //     QPushButton *btnOption = new QPushButton(option.text, this);
        
    //     btnOption->setMinimumHeight(100);
    //     btnOption->setMaximumHeight(110);
        
    //     connect(btnOption, &QPushButton::clicked, [mainWindow, option]()
    //             { mainWindow->gotoPage(option.pageId, option.text, option.data); });
    //     ui->scrollLayout->addWidget(btnOption);
       
    // }

    connect(ui->btnBack, &QPushButton::clicked, mainWindow, &MainWindow::btnBack_clicked);
    connect(ui->btnHome, &QPushButton::clicked, mainWindow, &MainWindow::btnHome_clicked);

}

ScreenName::~ScreenName()
{
    delete ui;
}

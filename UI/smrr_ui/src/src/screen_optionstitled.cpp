
#include "screen_optionstitled.h"
#include "./ui_screen_optionstitled.h"

#include <QDebug>

ScreenOptionsTitled::ScreenOptionsTitled(QWidget *parent, vector<Option> *options, QString text, string data)
    : QDialog(parent), ui(new Ui::ScreenOptionsTitled)
{
    ui->setupUi(this);

    MainWindow *mainWindow = static_cast<MainWindow *>(parent);

    qDebug() << "OptionsTitled window loaded";

    ui->label->setText(text);

    for (uint i = 0; i < options->size(); i++)
    {
        qDebug() << "screen_options:i" << i;

        Option option = options->at(i);
        QPushButton *btnOption = new QPushButton(option.text, this);

        // btnOption->setMinimumHeight(100);
        // btnOption->setMaximumHeight(110);
        btnOption->setFixedHeight(150);
        btnOption->setMinimumWidth(150);
        btnOption->setMaximumWidth(200);

        connect(btnOption, &QPushButton::clicked, [mainWindow, option]()
                { mainWindow->gotoPage(option.pageId, option.text, option.data, option.pubStr); });
        ui->scrollLayout->addWidget(btnOption);
    }

    connect(ui->btnBack, &QPushButton::clicked, mainWindow, &MainWindow::btnBack_clicked);
    connect(ui->btnHome, &QPushButton::clicked, mainWindow, &MainWindow::btnHome_clicked);
}

ScreenOptionsTitled::~ScreenOptionsTitled()
{
    delete ui;
}

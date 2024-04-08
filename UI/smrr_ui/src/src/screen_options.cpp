
#include "screen_options.h"
#include "./ui_screen_options.h"

#include <QDebug>

ScreenOptions::ScreenOptions(QWidget *parent, vector<Option> *options)
    : QDialog(parent), ui(new Ui::ScreenOptions)
{
    ui->setupUi(this);

    MainWindow *mainWindow = static_cast<MainWindow *>(parent);

    qDebug() << "options window loaded";
    // ui->label->setText(page->name);

    for (uint i = 0; i < options->size(); i++)
    {
        qDebug() << "screen_options:i" << i;

        Option option = options->at(i);
        QPushButton *btnOption = new QPushButton(option.text, this);
        // btnOption->setFixedHeight(60);
        btnOption->setMinimumHeight(100);
        btnOption->setMaximumHeight(110);

        // btnOption->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        connect(btnOption, &QPushButton::clicked, [mainWindow, option]()
                { mainWindow->gotoPage(option.pageId, option.text, option.data,option.pubStr); });
        ui->scrollLayout->addWidget(btnOption);
        // ui->label->setText(ui->label->text()+str);
        // qDebug()<<str;
    }

    connect(ui->btnBack, &QPushButton::clicked, mainWindow, &MainWindow::btnBack_clicked);
    connect(ui->btnHome, &QPushButton::clicked, mainWindow, &MainWindow::btnHome_clicked);
}

ScreenOptions::~ScreenOptions()
{
    delete ui;
}


#include "screen_face.h"
#include "./ui_screen_face.h"

#include <QDebug>

ScreenFace::ScreenFace(QWidget *parent /*TODO: , vector<Option> *options, QString text, PubStr *pubStr, string data */)
    : QDialog(parent), ui(new Ui::ScreenFace)
{
    ui->setupUi(this);

    MainWindow *mainWindow = static_cast<MainWindow *>(parent);

    qDebug() << "Face window loaded";

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

    connect(ui->btnBack, &QPushButton::clicked, static_cast<MainWindow *>(parent), &MainWindow::btnBack_clicked);
    connect(ui->btnHome, &QPushButton::clicked, static_cast<MainWindow *>(parent), &MainWindow::btnHome_clicked);

}

ScreenFace::~ScreenFace()
{
    delete ui;
}

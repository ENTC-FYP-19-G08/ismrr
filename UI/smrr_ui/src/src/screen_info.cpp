
#include "screen_info.h"
#include "./ui_screen_info.h"

#include <QDebug>

ScreenInfo::ScreenInfo(QWidget *parent , QString text,  string data )
    : QDialog(parent), ui(new Ui::ScreenInfo)
{
    ui->setupUi(this);

    MainWindow *mainWindow = static_cast<MainWindow *>(parent);

    qDebug() << "Info window loaded";

    ui->label->setText(text + " : " + QString::fromStdString(data));   

    connect(ui->btnBack, &QPushButton::clicked, mainWindow, &MainWindow::btnBack_clicked);
    connect(ui->btnHome, &QPushButton::clicked, mainWindow, &MainWindow::btnHome_clicked);

}

ScreenInfo::~ScreenInfo()
{
    delete ui;
}


#include "screen_map.h"
#include "./ui_screen_map.h"

#include <QDebug>

ScreenMap::ScreenMap(QWidget *parent ,QString text, string data)
    : QDialog(parent), ui(new Ui::ScreenMap)
{
    ui->setupUi(this);

    MainWindow *mainWindow = static_cast<MainWindow *>(parent);

    qDebug() << "Map window loaded";

    ui->label->setText(text);

    std_msgs::msg::String rosString;
    rosString.data = data;
    mainWindow->rosNode->pubGuideVerbal->publish(rosString);   

    connect(ui->btnBack, &QPushButton::clicked, mainWindow, &MainWindow::btnBack_clicked);
    connect(ui->btnHome, &QPushButton::clicked, mainWindow, &MainWindow::btnHome_clicked);

}

ScreenMap::~ScreenMap()
{
    delete ui;
}

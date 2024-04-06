
#include "screen_navigation.h"
#include "./ui_screen_navigation.h"

#include <QDebug>

ScreenNavigation::ScreenNavigation(QWidget *parent, QString text,string data)
    : QDialog(parent), ui(new Ui::ScreenNavigation)
{
    ui->setupUi(this);

    MainWindow *mainWindow = static_cast<MainWindow *>(parent);

    qDebug() << "Navigation window loaded";

    ui->label->setText(text);

    std_msgs::msg::String rosString;
    rosString.data = data;
    mainWindow->rosNode->pubGuideNavigation->publish(rosString);

    connect(ui->btnBack, &QPushButton::clicked, mainWindow, &MainWindow::btnBack_clicked);
    connect(ui->btnHome, &QPushButton::clicked, mainWindow, &MainWindow::btnHome_clicked);

}

ScreenNavigation::~ScreenNavigation()
{
    delete ui;
}

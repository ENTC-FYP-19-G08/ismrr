
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
}

ScreenNavigation::~ScreenNavigation()
{
    delete ui;
}

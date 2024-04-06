#ifndef SCREEN_NAVIGATION_H
#define SCREEN_NAVIGATION_H

#include <QDialog>
#include <QString>

#include <string>
// #include <vector>

#include "rclcomm.h"
// #include "option.h"

#include "mainwindow.h"

using namespace std;

namespace Ui
{
    class ScreenNavigation;
}

class ScreenNavigation : public QDialog
{
    Q_OBJECT

public:
    explicit ScreenNavigation(QWidget *parent = nullptr, QString text = "", string data = "");
    ~ScreenNavigation();

private:
    Ui::ScreenNavigation *ui;
};

#endif // SCREEN_NAVIGATION_H

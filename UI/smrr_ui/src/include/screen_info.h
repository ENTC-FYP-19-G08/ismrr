#ifndef SCREEN_INFO_H
#define SCREEN_INFO_H

#include <QDialog>
#include <QString>

// #include <string>
// #include <vector>

// #include "rclcomm.h"
// #include "option.h"

#include "mainwindow.h"

using namespace std;

namespace Ui {
class ScreenInfo;
}

class ScreenInfo : public QDialog
{
    Q_OBJECT

public:
    explicit ScreenInfo(QWidget *parent = nullptr  , QString text="", string data="");
    ~ScreenInfo();

private:
    Ui::ScreenInfo *ui;
};

#endif // SCREEN_INFO_H

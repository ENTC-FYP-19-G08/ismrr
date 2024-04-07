#ifndef SCREEN_HOME_H
#define SCREEN_HOME_H

#include <QDialog>
#include <QString>

// #include <string>
// #include <vector>

// #include "rclcomm.h"
// #include "option.h"

#include "mainwindow.h"

using namespace std;

namespace Ui {
class ScreenHome;
}

class ScreenHome : public QDialog
{
    Q_OBJECT

public:
    explicit ScreenHome(QWidget *parent = nullptr /*TODO: , vector<Option> *options=nullptr, QString text="", PubStr *pubStr = nullptr*, string data="" */);
    ~ScreenHome();

private:
    Ui::ScreenHome *ui;
};

#endif // SCREEN_HOME_H

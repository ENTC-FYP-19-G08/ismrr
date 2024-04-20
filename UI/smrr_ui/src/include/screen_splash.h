#ifndef SCREEN_SPLASH_H
#define SCREEN_SPLASH_H

#include <QDialog>
#include <QString>

// #include <string>
// #include <vector>

// #include "rclcomm.h"
// #include "option.h"

#include "mainwindow.h"

using namespace std;

namespace Ui {
class ScreenSplash;
}

class ScreenSplash : public QDialog
{
    Q_OBJECT

public:
    explicit ScreenSplash(QWidget *parent = nullptr /*TODO: , vector<Option> *options=nullptr, QString text="", PubStr *pubStr = nullptr*, string data="" */);
    ~ScreenSplash();

private:
    Ui::ScreenSplash *ui;
};

#endif // SCREEN_SPLASH_H

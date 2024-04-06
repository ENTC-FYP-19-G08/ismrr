#ifndef SCREEN_GREET_H
#define SCREEN_GREET_H

#include <QDialog>
#include <QString>

// #include <string>
// #include <vector>

// #include "rclcomm.h"
// #include "option.h"

#include "mainwindow.h"

using namespace std;

namespace Ui {
class ScreenGreet;
}

class ScreenGreet : public QDialog
{
    Q_OBJECT

public:
    explicit ScreenGreet(QWidget *parent = nullptr /*TODO: , vector<Option> *options=nullptr, QString text="", PubStr *pubStr = nullptr*, string data="" */);
    ~ScreenGreet();

private:
    Ui::ScreenGreet *ui;
};

#endif // SCREEN_GREET_H

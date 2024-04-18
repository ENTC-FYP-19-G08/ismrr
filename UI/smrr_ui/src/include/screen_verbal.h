#ifndef SCREEN_VERBAL_H
#define SCREEN_VERBAL_H

#include <QDialog>
#include <QString>

// #include <string>
// #include <vector>

// #include "rclcomm.h"
// #include "option.h"

#include "mainwindow.h"

using namespace std;

namespace Ui {
class ScreenVerbal;
}

class ScreenVerbal : public QDialog
{
    Q_OBJECT

public:
    explicit ScreenVerbal(QWidget *parent = nullptr /*TODO: , vector<Option> *options=nullptr, QString text="", PubStr *pubStr = nullptr*, string data="" */);
    ~ScreenVerbal();

private:
    Ui::ScreenVerbal *ui;
};

#endif // SCREEN_VERBAL_H

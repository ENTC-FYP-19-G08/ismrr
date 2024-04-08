#ifndef SCREEN_OPTIONSTITLED_H
#define SCREEN_OPTIONSTITLED_H

#include <QDialog>
#include <QString>

#include <string>
#include <vector>

// #include "rclcomm.h"
#include "option.h"

#include "mainwindow.h"

using namespace std;

namespace Ui {
class ScreenOptionsTitled;
}

class ScreenOptionsTitled : public QDialog
{
    Q_OBJECT

public:
    explicit ScreenOptionsTitled(QWidget *parent = nullptr , vector<Option> *options=nullptr, QString text="", string data="");
    ~ScreenOptionsTitled();

private:
    Ui::ScreenOptionsTitled *ui;
};

#endif // SCREEN_OPTIONSTITLED_H

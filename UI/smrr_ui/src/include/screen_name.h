#ifndef SCREEN_NAME_H
#define SCREEN_NAME_H

#include <QDialog>
#include <QString>

#include <string>
// #include <vector>

// #include "rclcomm.h"
// #include "option.h"

#include "mainwindow.h"

using namespace std;

namespace Ui {
class ScreenName;
}

class ScreenName : public QDialog
{
    Q_OBJECT

public:
    explicit ScreenName(QWidget *parent = nullptr /*TODO: , vector<Option> *options=nullptr, QString text="", PubStr *pubStr = nullptr*, string data="" */);
    ~ScreenName();

private:
    Ui::ScreenName *ui;
};

#endif // SCREEN_NAME_H

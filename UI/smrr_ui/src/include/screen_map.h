#ifndef SCREEN_MAP_H
#define SCREEN_MAP_H

#include <QDialog>
#include <QString>

// #include <string>
// #include <vector>

// #include "rclcomm.h"
// #include "option.h"

#include "mainwindow.h"

using namespace std;

namespace Ui {
class ScreenMap;
}

class ScreenMap : public QDialog
{
    Q_OBJECT

public:
    explicit ScreenMap(QWidget *parent = nullptr /*TODO: , vector<Option> *options=nullptr, QString text="", PubStr *pubStr = nullptr*/);
    ~ScreenMap();

private:
    Ui::ScreenMap *ui;
};

#endif // SCREEN_MAP_H

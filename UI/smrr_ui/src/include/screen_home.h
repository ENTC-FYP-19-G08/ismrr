#ifndef SCREEN_HOME_H
#define SCREEN_HOME_H

#include <QDialog>
#include <QString>

// #include <string>
// #include <vector>

// #include "rclcomm.h"
#include "option.h"

#include "mainwindow.h"

using namespace std;

namespace Ui
{
    class ScreenHome;
}

class ScreenHome : public QDialog
{
    Q_OBJECT

public:
    MainWindow *mainWindow;
    explicit ScreenHome(QWidget *parent = nullptr);
    ~ScreenHome();

public slots:
    void onTrigger(QString);

private:
    Ui::ScreenHome *ui;
};

#endif // SCREEN_HOME_H

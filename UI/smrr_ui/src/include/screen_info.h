#ifndef SCREEN_INFO_H
#define SCREEN_INFO_H

#include <QDialog>
#include <vector>
#include <QString>
#include "page.h"
#include "mainwindow.h"

using namespace std;

namespace Ui {
class ScreenInfo;
}

class ScreenInfo : public QDialog
{
    Q_OBJECT

public:
    explicit ScreenInfo(QWidget *parent = nullptr, Page *page = nullptr);
    ~ScreenInfo();

private:
    Ui::ScreenInfo *ui;
};

#endif // SCREEN_INFO_H

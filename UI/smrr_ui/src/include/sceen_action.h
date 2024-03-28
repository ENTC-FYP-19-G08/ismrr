#ifndef SCREEN_ACTION_H
#define SCREEN_ACTION_H

#include <QDialog>
#include <vector>
#include <QString>
#include "page.h"
#include "mainwindow.h"

using namespace std;

namespace Ui {
class SceenAction;
}

class SceenAction : public QDialog
{
    Q_OBJECT

public:
    explicit SceenAction(QWidget *parent = nullptr, Page *page = nullptr);
    ~SceenAction();

private:
    Ui::SceenAction *ui;
};

#endif // SCREEN_ACTION_H

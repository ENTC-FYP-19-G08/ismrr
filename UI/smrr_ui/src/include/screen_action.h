#ifndef SCREEN_ACTION_H
#define SCREEN_ACTION_H

#include <QDialog>
#include <vector>
#include <QString>
#include "page.h"
#include "mainwindow.h"

using namespace std;

namespace Ui {
class ScreenAction;
}

class ScreenAction : public QDialog
{
    Q_OBJECT

public:
    explicit ScreenAction(QWidget *parent = nullptr, Page *page = nullptr);
    ~ScreenAction();

private:
    Ui::ScreenAction *ui;
};

#endif // SCREEN_ACTION_H

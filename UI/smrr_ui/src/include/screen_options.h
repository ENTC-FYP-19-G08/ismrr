#ifndef SCREEN_OPTIONS_H
#define SCREEN_OPTIONS_H

#include <QDialog>
#include <vector>
#include <QString>
#include "page.h"
#include "option.h"
#include "mainwindow.h"

using namespace std;

namespace Ui {
class ScreenOptions;
}

class ScreenOptions : public QDialog
{
    Q_OBJECT

public:
    explicit ScreenOptions(QWidget *parent = nullptr, vector<Option> *options=nullptr);
    ~ScreenOptions();

private:
    Ui::ScreenOptions *ui;
};

#endif // SCREEN_OPTIONS_H

#ifndef SCREEN_OPTIONS_H
#define SCREEN_OPTIONS_H

#include <QDialog>
#include <vector>
#include <QString>
#include "page.h"
#include "mainwindow.h"

using namespace std;

namespace Ui {
class SceenOptions;
}

class SceenOptions : public QDialog
{
    Q_OBJECT

public:
    explicit SceenOptions(QWidget *parent = nullptr, Page *page = nullptr);
    ~SceenOptions();

private:
    Ui::SceenOptions *ui;
};

#endif // OPTIONSWINDOW_H

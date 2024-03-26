#ifndef OPTIONSWINDOW_H
#define OPTIONSWINDOW_H

#include <QDialog>
#include <vector>
#include <QString>
#include "page.h"
#include "mainwindow.h"

using namespace std;

namespace Ui {
class OptionsWindow;
}

class OptionsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsWindow(QWidget *parent = nullptr, Page *page = nullptr);
    ~OptionsWindow();

private:
    Ui::OptionsWindow *ui;
};

#endif // OPTIONSWINDOW_H

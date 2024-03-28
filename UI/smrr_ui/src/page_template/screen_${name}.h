#ifndef SCREEN_${NAME}_H
#define SCREEN_${NAME}_H

#include <QDialog>
#include <vector>
#include <QString>
#include "page.h"
#include "mainwindow.h"

using namespace std;

namespace Ui {
class Screen${Name};
}

class Screen${Name} : public QDialog
{
    Q_OBJECT

public:
    explicit Screen${Name}(QWidget *parent = nullptr, Page *page = nullptr);
    ~Screen${Name}();

private:
    Ui::Screen${Name} *ui;
};

#endif // SCREEN_${NAME}_H

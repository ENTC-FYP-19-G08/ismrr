#ifndef SCREEN_${NAME}_H
#define SCREEN_${NAME}_H

#include <QDialog>
#include <vector>
#include <QString>
#include "page.h"
#include "mainwindow.h"

using namespace std;

namespace Ui {
class Sceen${Name};
}

class Sceen${Name} : public QDialog
{
    Q_OBJECT

public:
    explicit Sceen${Name}(QWidget *parent = nullptr, Page *page = nullptr);
    ~Sceen${Name}();

private:
    Ui::Sceen${Name} *ui;
};

#endif // SCREEN_${NAME}_H

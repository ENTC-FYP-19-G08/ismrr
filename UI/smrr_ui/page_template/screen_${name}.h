#ifndef SCREEN_${NAME}_H
#define SCREEN_${NAME}_H

#include <QDialog>
#include <QString>

// #include <string>
// #include <vector>

// #include "rclcomm.h"
// #include "option.h"

#include "mainwindow.h"

using namespace std;

namespace Ui {
class Screen${Name};
}

class Screen${Name} : public QDialog
{
    Q_OBJECT

public:
    explicit Screen${Name}(QWidget *parent = nullptr /*TODO: , vector<Option> *options=nullptr, QString text="", PubStr *pubStr = nullptr*/);
    ~Screen${Name}();

private:
    Ui::Screen${Name} *ui;
};

#endif // SCREEN_${NAME}_H

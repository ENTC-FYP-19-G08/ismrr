#ifndef SCREEN_FACE_H
#define SCREEN_FACE_H

#include <QDialog>
#include <QString>

// #include <string>
// #include <vector>

// #include "rclcomm.h"
// #include "option.h"

#include "mainwindow.h"

using namespace std;

namespace Ui {
class ScreenFace;
}

class ScreenFace : public QDialog
{
    Q_OBJECT

public:
    explicit ScreenFace(QWidget *parent = nullptr /*TODO: , vector<Option> *options=nullptr, QString text="", PubStr *pubStr = nullptr*, string data="" */);
    ~ScreenFace();

private:
    Ui::ScreenFace *ui;
};

#endif // SCREEN_FACE_H

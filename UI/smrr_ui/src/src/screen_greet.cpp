
#include "screen_greet.h"
#include "./ui_screen_greet.h"

#include <QDebug>

ScreenGreet::ScreenGreet(QWidget *parent /*TODO: , vector<Option> *options, QString text, PubStr *pubStr, string data */)
    : QDialog(parent), ui(new Ui::ScreenGreet)
{
    ui->setupUi(this);

    MainWindow *mainWindow = static_cast<MainWindow *>(parent);

    qDebug() << "Greet window loaded";

    // ui->label->setText();

    

    connect(ui->btnBack, &QPushButton::clicked, mainWindow, &MainWindow::btnBack_clicked);
    connect(ui->btnHome, &QPushButton::clicked, mainWindow, &MainWindow::btnHome_clicked);

}

ScreenGreet::~ScreenGreet()
{
    delete ui;
}

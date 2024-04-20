
#include "screen_splash.h"
#include "./ui_screen_splash.h"

#include <QDebug>


ScreenSplash::ScreenSplash(QWidget *parent /*TODO: , vector<Option> *options, QString text, PubStr *pubStr, string data */)
    : QDialog(parent), ui(new Ui::ScreenSplash)
{
    ui->setupUi(this);

    qDebug() << "Splash window loaded";
    // movie= new QMovie(RES_PATH "loading4.gif");     
    // ui->labelImg->setMovie(movie);
    // movie->start();
}

ScreenSplash::~ScreenSplash()
{   
    delete ui;
    // delete movie;
}

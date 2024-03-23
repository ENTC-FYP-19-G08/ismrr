#include "optionswindow.h"
#include "./ui_optionswindow.h"
#include <QDebug>


OptionsWindow::OptionsWindow(QWidget *parent, vector<QString>* strings)
    : QDialog(parent)
    , ui(new Ui::OptionsWindow)
{
    ui->setupUi(this);
    
    qDebug()<<"options window loaded";
     for (const QString& str : *strings) {
         ui->label->setText(ui->label->text()+str);
         qDebug()<<str;
    }
}

OptionsWindow::~OptionsWindow()
{
    delete ui;
}


#include "optionswindow.h"
#include "./ui_optionswindow.h"

#include <QDebug>

OptionsWindow::OptionsWindow(QWidget *parent, Page *page)
    : QDialog(parent)
    , ui(new Ui::OptionsWindow)
{
    ui->setupUi(this);

    qDebug()<<"options window loaded";
     for (uint i = 0; i < page->options.size(); i++) {

        QPushButton *btnOption=new QPushButton(MainWindow::pages->at(page->options.at(i)).name);
        btnOption->setFixedHeight(60);
        connect(btnOption, &QPushButton::clicked, [i]() {
            MainWindow::btnOk_clicked(i);
            qDebug() << "Button " << i << " clicked";
        });
        ui->scrollLayout->addWidget(btnOption);
        // ui->label->setText(ui->label->text()+str);
        // qDebug()<<str;
    }

    connect(ui->btnBack,&QPushButton::clicked,&MainWindow::btnBack_clicked);
}

OptionsWindow::~OptionsWindow()
{
    delete ui;
}

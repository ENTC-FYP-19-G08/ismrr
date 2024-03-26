
#include "sceen_options.h"
#include "./ui_sceen_options.h"

#include <QDebug>

SceenOptions::SceenOptions(QWidget *parent, Page *page)
    : QDialog(parent)
    , ui(new Ui::SceenOptions)
{
    ui->setupUi(this);

    qDebug()<<"options window loaded";
     for (uint i = 0; i < page->nextPageIds.size(); i++) {
        PAGE_ID nextPageId=page->nextPageIds.at(i);
        QPushButton *btnOption=new QPushButton(MainWindow::pages->at(nextPageId).name);
        btnOption->setFixedHeight(60);
        connect(btnOption, &QPushButton::clicked, [nextPageId,parent]() {
            static_cast<MainWindow*>(parent)->btnNext_clicked(nextPageId);
            qDebug() << "Button " << nextPageId << " clicked";
        });
        ui->scrollLayout->addWidget(btnOption);
        // ui->label->setText(ui->label->text()+str);
        // qDebug()<<str;
    }

    connect(ui->btnBack,&QPushButton::clicked,static_cast<MainWindow*>(parent),&MainWindow::btnBack_clicked);
}

SceenOptions::~SceenOptions()
{
    delete ui;
}

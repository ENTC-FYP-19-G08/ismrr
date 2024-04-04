
#include "screen_info.h"
#include "./ui_screen_info.h"

#include <QDebug>

ScreenInfo::ScreenInfo(QWidget *parent, Page *page)
    : QDialog(parent), ui(new Ui::ScreenInfo)
{
    ui->setupUi(this);

    qDebug() << "info window loaded";
    ui->label->setText(page->name);
    for (uint i = 0; i < page->nextPageIds.size(); i++)
    {
        PAGE_ID nextPageId = page->nextPageIds.at(i);
        QPushButton *btnOption = new QPushButton(MainWindow::pages->at(nextPageId).name,this);
        // btnOption->setFixedHeight(60);
        btnOption->setMinimumHeight(100);
        btnOption->setMaximumHeight(110);

        // btnOption->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum); 
        connect(btnOption, &QPushButton::clicked, [nextPageId, parent](){
            static_cast<MainWindow*>(parent)->btnNext_clicked(nextPageId);
            qDebug() << "Button " << nextPageId << " clicked"; });
        ui->scrollLayout->addWidget(btnOption);
        // ui->label->setText(ui->label->text()+str);
        // qDebug()<<str;
    }

    connect(ui->btnBack, &QPushButton::clicked, static_cast<MainWindow *>(parent), &MainWindow::btnBack_clicked);
    connect(ui->btnHome, &QPushButton::clicked, static_cast<MainWindow *>(parent), &MainWindow::btnHome_clicked);

}

ScreenInfo::~ScreenInfo()
{
    delete ui;
}


#include "screen_options.h"
#include "./ui_screen_options.h"

#include <QDebug>

ScreenOptions::ScreenOptions(QWidget *parent, Page *page)
    : QDialog(parent), ui(new Ui::ScreenOptions)
{
    ui->setupUi(this);

    MainWindow *mainWindow=static_cast<MainWindow*>(parent);

    qDebug() << "options window loaded";
    ui->label->setText(page->name);
    for (uint i = 0; i < page->nextPageIds.size(); i++)
    {
        PAGE_ID nextPageId = page->nextPageIds.at(i);
        QPushButton *btnOption = new QPushButton(MainWindow::pages->at(nextPageId).name,this);
        // btnOption->setFixedHeight(60);
        btnOption->setMinimumHeight(100);
        btnOption->setMaximumHeight(110);

        // btnOption->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum); 
        connect(btnOption, &QPushButton::clicked, [nextPageId, mainWindow](){
            mainWindow->btnNext_clicked(nextPageId);
            qDebug() << "Button " << nextPageId << " clicked"; });
        ui->scrollLayout->addWidget(btnOption);
        // ui->label->setText(ui->label->text()+str);
        // qDebug()<<str;
    }

    connect(ui->btnBack, &QPushButton::clicked, mainWindow, &MainWindow::btnBack_clicked);
    connect(ui->btnHome, &QPushButton::clicked, mainWindow, &MainWindow::btnHome_clicked);

}

ScreenOptions::~ScreenOptions()
{
    delete ui;
}

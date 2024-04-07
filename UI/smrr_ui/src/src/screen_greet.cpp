
#include "screen_greet.h"
#include "./ui_screen_greet.h"

#include <QDebug>

ScreenGreet::ScreenGreet(QWidget *parent, QString text)
    : QDialog(parent), ui(new Ui::ScreenGreet)
{
    ui->setupUi(this);

    MainWindow *mainWindow = static_cast<MainWindow *>(parent);

    qDebug() << "Greet window loaded";

    ui->label->setText(text);

    QPushButton *btnOption = new QPushButton("Options", this);   
    btnOption->setMinimumHeight(100);
    btnOption->setMaximumHeight(110);  
    connect(btnOption, &QPushButton::clicked, [mainWindow]()
            { mainWindow->gotoPage(PAGE_GUIDE); });
    ui->scrollLayout->addWidget(btnOption);

    connect(ui->btnBack, &QPushButton::clicked, mainWindow, &MainWindow::btnBack_clicked);
    connect(ui->btnHome, &QPushButton::clicked, mainWindow, &MainWindow::btnHome_clicked);
}

ScreenGreet::~ScreenGreet()
{
    qDebug() << "SCREEN_GREET_DECONSTRUCTOR";
    delete ui;
}


#include "screen_action.h"
#include "./ui_screen_action.h"

#include <QDebug>

ScreenAction::ScreenAction(QWidget *parent, Page *page)
    : QDialog(parent), ui(new Ui::ScreenAction)
{
    ui->setupUi(this);
    MainWindow *mainWindow = static_cast<MainWindow *>(parent);
    qDebug() << "action window loaded";
    ui->label->setText(page->name +" : "+ QString::fromStdString(page->rosData));

    mainWindow->rosNode->sendRosData(page->rosData);

    connect(ui->btnBack, &QPushButton::clicked, mainWindow, &MainWindow::btnBack_clicked);
    connect(ui->btnHome, &QPushButton::clicked, mainWindow, &MainWindow::btnHome_clicked);
}

ScreenAction::~ScreenAction()
{
    delete ui;
}

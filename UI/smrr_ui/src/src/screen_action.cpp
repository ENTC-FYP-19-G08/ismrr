
#include "screen_action.h"
#include "./ui_screen_action.h"

#include <QDebug>

ScreenAction::ScreenAction(QWidget *parent, Page *page)
    : QDialog(parent), ui(new Ui::ScreenAction)
{
    ui->setupUi(this);

    qDebug() << "action window loaded";
    ui->label->setText(page->name+QString::fromStdString(page->rosTopic)+QString::number(page->rosData));
    

    connect(ui->btnBack, &QPushButton::clicked, static_cast<MainWindow *>(parent), &MainWindow::btnBack_clicked);
    connect(ui->btnHome, &QPushButton::clicked, static_cast<MainWindow *>(parent), &MainWindow::btnHome_clicked);

}

ScreenAction::~ScreenAction()
{
    delete ui;
}

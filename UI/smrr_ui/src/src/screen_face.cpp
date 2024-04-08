
#include "screen_face.h"
#include "./ui_screen_face.h"

#include <QDebug>

ScreenFace::ScreenFace(QWidget *parent)
    : QDialog(parent), ui(new Ui::ScreenFace)
{
    ui->setupUi(this);

    mainWindow = static_cast<MainWindow *>(parent);

    qDebug() << "Face window loaded";

    connect(mainWindow->rosNode, &rclcomm::onUsername, this, &ScreenFace::onUsername);

    connect(ui->btnSkip, &QPushButton::clicked, [this]()
            {   mainWindow->publishStr(mainWindow->rosNode->pubUnknownUsername,"<SKIP>");
                mainWindow->gotoPage(PAGE_GREET); });

    connect(ui->btnBack, &QPushButton::clicked, mainWindow, &MainWindow::btnBack_clicked);
    connect(ui->btnHome, &QPushButton::clicked, mainWindow, &MainWindow::btnHome_clicked);
}

ScreenFace::~ScreenFace()
{
    delete ui;
}

void ScreenFace::onUsername(QString qdata)
{
    qDebug() << "ScreenFace Sub:" << qdata;
    // ui->label->setText(qdata);
    if (qdata == "unknown")
        mainWindow->gotoPage(PAGE_NAME);
    else
        mainWindow->gotoPage(PAGE_GREET, qdata);
}

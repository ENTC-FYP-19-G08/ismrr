
#include "screen_map.h"
#include "./ui_screen_map.h"

#include <QDebug>
#include <QPixmap>
#include <QLabel>
ScreenMap::ScreenMap(QWidget *parent, QString text, string data)
    : QDialog(parent), ui(new Ui::ScreenMap)
{
    ui->setupUi(this);

    MainWindow *mainWindow = static_cast<MainWindow *>(parent);

    qDebug() << "Map window loaded";

    QString imgsPath = "imgs/";

    QPixmap pixmap(imgsPath + QString::fromStdString(data) + ".jpg");
    if (pixmap.isNull())
    {
        // Handle error if image loading fails (e.g., display a message)
        qDebug() << "Error loading image";
        ui->labelImg->setText("Image is not added yet . . .");
    }
    else
    {
        QPixmap scaledPixmap = pixmap.scaled(ui->labelImg->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        ui->labelImg->setPixmap(scaledPixmap);
    }

    ui->label->setText(text + ":" + QString::fromStdString(data));

    std_msgs::msg::String rosString;
    rosString.data = data;
    mainWindow->rosNode->pubGuideVerbal->publish(rosString);

    connect(ui->btnBack, &QPushButton::clicked, mainWindow, &MainWindow::btnBack_clicked);
    connect(ui->btnHome, &QPushButton::clicked, mainWindow, &MainWindow::btnHome_clicked);
}

ScreenMap::~ScreenMap()
{
    delete ui;
}

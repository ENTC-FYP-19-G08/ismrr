
#include "screen_name.h"
#include "./ui_screen_name.h"

#include <QDebug>

ScreenName::ScreenName(QWidget *parent /*TODO: , vector<Option> *options, QString text, PubStr *pubStr, string data */)
    : QDialog(parent), ui(new Ui::ScreenName)
{
    ui->setupUi(this);

    MainWindow *mainWindow = static_cast<MainWindow *>(parent);

    qDebug() << "Name window loaded";

    
    QLineEdit *txtName=ui->txtName;

    connect(ui->btnSave, &QPushButton::clicked, [mainWindow, txtName]()
            { 
                QString qname=txtName->text();
                qDebug()<<"NAME:"<<qname;
                std_msgs::msg::String rosString;
                rosString.data = qname.toStdString();
                mainWindow->rosNode->pubUnknownUsername->publish(rosString);
                mainWindow->gotoPage(PAGE_GREET,qname); });

    connect(ui->btnSkip, &QPushButton::clicked, [mainWindow]()
            {  
                qDebug()<<"NAME: SKIP";
                std_msgs::msg::String rosString;
                rosString.data = "SKIP";
                mainWindow->rosNode->pubUnknownUsername->publish(rosString);
                mainWindow->gotoPage(PAGE_GREET); });

    connect(ui->btnBack, &QPushButton::clicked, mainWindow, &MainWindow::btnBack_clicked);
    connect(ui->btnHome, &QPushButton::clicked, mainWindow, &MainWindow::btnHome_clicked);
}

ScreenName::~ScreenName()
{
    delete ui;
}

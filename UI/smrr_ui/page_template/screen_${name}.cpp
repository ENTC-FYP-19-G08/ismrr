
#include "screen_${name}.h"
#include "./ui_screen_${name}.h"

#include <QDebug>

Screen${Name}::Screen${Name}(QWidget *parent /*TODO: , vector<Option> *options, QString text, PubStr *pubStr */)
    : QDialog(parent), ui(new Ui::Screen${Name})
{
    ui->setupUi(this);

      MainWindow *mainWindow = static_cast<MainWindow *>(parent);

    qDebug() << "${Name} window loaded";

    // ui->label->setText();

    for (uint i = 0; i < options->size(); i++)
    {
        qDebug() << "screen_options:i" << i;

        Option option = options->at(i);
        QPushButton *btnOption = new QPushButton(option.text, this);
        
        btnOption->setMinimumHeight(100);
        btnOption->setMaximumHeight(110);
        
        connect(btnOption, &QPushButton::clicked, [mainWindow, option]()
                { mainWindow->gotoPage(option.pageId, option.text, option.data); });
        ui->scrollLayout->addWidget(btnOption);
       
    }

    connect(ui->btnBack, &QPushButton::clicked, static_cast<MainWindow *>(parent), &MainWindow::btnBack_clicked);
    connect(ui->btnHome, &QPushButton::clicked, static_cast<MainWindow *>(parent), &MainWindow::btnHome_clicked);

}

Screen${Name}::~Screen${Name}()
{
    delete ui;
}

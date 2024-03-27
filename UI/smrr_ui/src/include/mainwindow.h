/*
 * @Author: chengyangkj
 * @Date: 2021-10-30 02:09:08
 * @LastEditTime: 2021-12-01 06:01:17
 * @LastEditors: chengyangkj
 * @Description:
 * @FilePath: /ros2_qt_demo/include/ros2_qt_demo/mainwindow.h
 * https://github.com/chengyangkj
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

// #include "rclcomm.h"
#include "page.h"
#include <iostream>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static vector<Page> *pages;
    QWidget *screen=nullptr;
    // static vector<int> tmp;
    
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QWidget *createScreen(Page *page);
    
    

private:
    Ui::MainWindow *ui;
    // rclcomm *commNode;
    void generateAllPages();

public slots:
    void updateTopicInfo(QString);    
    void btnNext_clicked(PAGE_ID nextPageId);
    void btnBack_clicked();
    void btnHome_clicked();
    // void on_btnOk_clicked(int);
    // void on_btnBack_clicked();
};
#endif // MAINWINDOW_H

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
#include <QPushButton>

#include <string>
#include <map>
#include <unordered_set>

#include "rclcomm.h"
#include "page.h"
#include "option.h"
#include "widget_border.h"
#include "widget_blinker.h"

using namespace std;

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
    QWidget *currentScreen = nullptr;
    WidgetBorder *border=nullptr;
    WidgetBlinker *blinker=nullptr;
    QPushButton *btn=nullptr;
    

    bool currentScreenHist = true;

    map<string, QString> locationMap;
    unordered_set<string> reachableLocations;
    rclcomm *rosNode;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QWidget *createScreen(Page *page);
    void showScreen(QWidget *screen, bool screenHist = true);
    void generateLocationData();
    // void publishStr(PubStr pubStr,QString data);
    void publishStr(PubStr pubStr,string data);
    void loadOptionsFromPrefix(vector<Option> *options,string prefix);
    

private:
    Ui::MainWindow *ui;

public slots:
    void updateTopicInfo(QString);
    void gotoPage(PageId pageId, QString text = "", string data = "", PubStr pubStr = nullptr);
    void btnBack_clicked();
    void btnHome_clicked();
    void onGuideNavigationResult(QString);
    void onGuideOptions(QString);
    void onChangeState(QString);
};
#endif // MAINWINDOW_H


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QString>

#include <string>
#include <map>
#include <unordered_set>

// ////////////////////////////  change to SMRR_UI abs path
#define UI_BASE_PATH ""


#define RES_PATH UI_BASE_PATH "imgs/"

#include "rclcomm.h"
#include "page.h"
#include "option.h"
#include "widget_border.h"
#include "widget_blinker.h"
#include "widget_toggler.h"

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

    WidgetBlinker *border = nullptr;
    WidgetToggler *listenToggler = nullptr;

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
    void publishStr(PubStr pubStr, string data);
    void loadOptionsFromPrefix(vector<Option> *options, string prefix);
    void gotoPage(PageId pageId, QString text = "", string data = "", PubStr pubStr = nullptr);

private:
    Ui::MainWindow *ui;

public slots:
    void updateTopicInfo(QString);

    void btnBack_clicked();
    void btnHome_clicked();
    void listenToggler_toggled(bool);
    
    void onGuideNavigationResult(QString);
    void onGuideOptions(QString);
    void onChangeState(QString);
};
#endif // MAINWINDOW_H

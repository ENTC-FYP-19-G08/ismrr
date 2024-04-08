#ifndef PAGE_H
#define PAGE_H

#include <QString>
#include <string>
#include <vector>
#include <QtWidgets/qwidget.h>
#include "rclcomm.h"

using namespace std;

enum SCREEN_ID
{
    SCREEN_MENU_OPTIONS,
    SCREEN_ACTION,
    SCREEN_GUIDE_OPTIONS
    // WINDOW1,WINDOW2,WINDOW3
};

class Page
{
private:
    /* data */
public:
    QString name;
    SCREEN_ID screenId;
    vector<int> nextPageIds;
    PubStr pubStr;
    string rosData;
    bool noHist;

    Page(QString name, SCREEN_ID screenId, vector<int> nextPageIds, PubStr pubStr = nullptr, string rosData = "", bool noHist = false);
    ~Page();
};

#endif
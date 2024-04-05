#ifndef PAGE_H
#define PAGE_H

#include <QString>
#include <iostream>
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

enum PAGE_ID_NOT_DATA_TYPE
{
    PAGE_HOME,
    PAGE_BASIC_OPTIONS,
    PAGE_GUIDE,
    PAGE_GUIDE_OFFICE,
    PAGE_GUIDE_LABS,
    PAGE_GUIDE_LAB_ANALOG,
    PAGE_GUIDE_LAB_DIGITAL,
    PAGE_GUIDE_LAB_TELECOM,
    PAGE_GUIDE_LAB_BM,
    PAGE_GUIDE_HALLS,
    PAGE_GUIDE_HALL_ENTC1,
    PAGE_GUIDE_HALL_PG,
    PAGE_GUIDE_HALL_3P5,
    PAGE_GUIDE_OTHER,
    PAGE_GUIDE_OTHER_WASHROOMS,
    PAGE_GUIDE_OTHER_LIFT,
    PAGE_GUIDE_OTHER_COMMON_LOWER,
    PAGE_GUIDE_OTHER_COMMON_UPPER,

    PAGE_MEET,
    PAGE_ABOUT_DEPARTMENT,
    PAGE_GUIDE_OPTIONS
    // PAGE1,PAGE2,PAGE3,PAGE4,PAGE5,PAGE6,PAGE7
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
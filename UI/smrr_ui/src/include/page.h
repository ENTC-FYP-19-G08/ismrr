#ifndef PAGE_H
#define PAGE_H

#include <QString>
#include <iostream>
#include <vector>
#include <QtWidgets/qwidget.h>

using namespace std;

enum SCREEN_ID
{
    // WINDOW_OPTIONS
    WINDOW1,WINDOW2,WINDOW3
};

enum PAGE_ID{
    // PAGE_HOME=0,
    // PAGE_BASIC_OPTIONS=1,
    // PAGE_GUIDE=2,
    // PAGE_MEET=3,
    // PAGE_ABOUT_DEPARTMENT=4,
    // PAGE_LABS=5
    PAGE1,PAGE2,PAGE3,PAGE4,PAGE5,PAGE6,PAGE7
};

class Page
{
private:
    /* data */
public:
    QString name;
    SCREEN_ID screenId;
    vector<PAGE_ID> nextPageIds;
    string rosTopic;
    int rosData;
    Page(int i);
    Page(QString name, SCREEN_ID screenId, vector<PAGE_ID> nextPageIds, string rosTopic, uint rosData);
    ~Page();
};

#endif
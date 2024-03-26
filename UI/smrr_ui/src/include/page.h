#ifndef PAGE_H
#define PAGE_H

#include <QString>
#include <iostream>
#include <vector>
#include <QtWidgets/qwidget.h>

using namespace std;

enum WINDOW_ID
{
    OPTIONS_WINDOW
};

enum PAGE_ID{
    HOME_PAGE,
    
};

class Page
{
private:
    /* data */
public:
    QString name;
    WINDOW_ID windowId;
    vector<uint> options;
    string rosTopic;
    int rosData;
    Page(int i);
    Page(QString name, WINDOW_ID windowId, vector<uint> options, string rosTopic, uint rosData);
    ~Page();
};

#endif
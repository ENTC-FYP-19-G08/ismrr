#ifndef OPTION_H
#define OPTION_H

#include <QString>
#include <vector>
#include "rclcomm.h"

using namespace std;

enum PageId
{
    PAGE_HOME,
    PAGE_FACE,
    PAGE_NAME,
    PAGE_BASIC_OPTIONS,
    PAGE_GUIDE,
    PAGE_GUIDE_LABS,
    PAGE_GUIDE_HALLS,
    PAGE_GUIDE_OTHER,
    PAGE_GUIDE_OPTIONS,
    PAGE_MEET,
    PAGE_ABOUT,
    PAGE_ABOUT_DEPARTMENT,
    PAGE_NAVIGATION,
    PAGE_VERBAL,
    PAGE_INFO,
    PAGE_SETTINGS,
    PAGE_SPLASH,

    PAGE_DEMO,

    PAGE_DEMO_GESTURES,
    ACTION_DEMO_GESTURES,
    
    PAGE_DEMO_NAVIGATION,
    PAGE_DEMO_CONVERSATION,

    PAGE_ACTION_RES_1280x720,
    PAGE_ACTION_RES_640x480,
    PAGE_ACTION_MINIMIZE,
    PAGE_ACTION_FULLSCREEN
};

class Option
{

public:
    PageId pageId;
    QString text;
    string data;
    PubStr pubStr;
    Option(PageId pageId, QString text = "", string data = "", PubStr pubStr = nullptr);
};

#endif // OPTION_H
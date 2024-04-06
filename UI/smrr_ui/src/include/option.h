#ifndef OPTION_H
#define OPTION_H

#include <QString>
#include <vector>

using namespace std;

enum PageId
{
    PAGE_HOME,
    PAGE_BASIC_OPTIONS,
    PAGE_GUIDE,
    PAGE_GUIDE_LABS,
    PAGE_GUIDE_HALLS,
    PAGE_GUIDE_OTHER,
    PAGE_GUIDE_OPTIONS,
    PAGE_MEET,
    PAGE_ABOUT_DEPARTMENT

};

class Option
{

public:
    PageId pageId;
    QString text;
    string data;
    Option(PageId pageId, QString text, string data = "");
};

#endif // OPTION_H
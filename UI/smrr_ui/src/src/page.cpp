#include "page.h"



Page::Page(QString name,SCREEN_ID screenId, vector<PAGE_ID> nextPageIds, string rosData): 
name(name),screenId(screenId),nextPageIds(nextPageIds),rosData(rosData)
{
}

Page::~Page()
{
}
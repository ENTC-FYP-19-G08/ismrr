#include "page.h"



Page::Page(QString name,SCREEN_ID screenId, vector<PAGE_ID> nextPageIds,PubStr pubStr, string rosData): 
name(name),screenId(screenId),nextPageIds(nextPageIds),pubStr(pubStr),rosData(rosData)
{
}

Page::~Page()
{
}
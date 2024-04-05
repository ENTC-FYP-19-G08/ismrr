#include "page.h"

Page::Page(QString name,SCREEN_ID screenId, vector<int> nextPageIds,PubStr pubStr, string rosData, bool noHist): 
name(name),screenId(screenId),nextPageIds(nextPageIds),pubStr(pubStr),rosData(rosData),noHist(noHist)
{
}

Page::~Page()
{
}
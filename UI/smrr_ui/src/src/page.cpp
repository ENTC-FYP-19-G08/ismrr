#include "page.h"



Page::Page(QString name,SCREEN_ID screenId, vector<PAGE_ID> nextPageIds, string rosTopic, uint rosData): 
name(name),screenId(screenId),nextPageIds(nextPageIds),rosTopic(rosTopic),rosData(rosData)
{
}

Page::~Page()
{
}
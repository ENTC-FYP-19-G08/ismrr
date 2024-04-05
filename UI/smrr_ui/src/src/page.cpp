#include "page.h"



Page::Page(QString name,SCREEN_ID screenId, vector<int> nextPageIds,void (*emitPublish)(string), string rosData, bool noHist): 
name(name),screenId(screenId),nextPageIds(nextPageIds),emitPublish(emitPublish),rosData(rosData),noHist(noHist)
{
}

Page::~Page()
{
}
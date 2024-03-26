#include "page.h"

Page::Page(int i){}

Page::Page(QString name,WINDOW_ID windowId, vector<uint> options, string rosTopic, uint rosData): name(name),rosTopic(rosTopic),rosData(rosData)
{
}

Page::~Page()
{
}
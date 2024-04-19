#include "widget_border.h"
#include <QPainter>

WidgetBorder::WidgetBorder(QWidget *parent) : QWidget(parent), m_visible(true)
{
    setFixedSize(parent->size()); // Adjust the size as per your requirement
}

WidgetBorder::~WidgetBorder() {}

void WidgetBorder::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPen pen(Qt::green); 
    pen.setWidth(50);
    painter.setPen(pen);
    painter.drawRect(rect());
}

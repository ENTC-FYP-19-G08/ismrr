#include "widget_border.h"
#include <QPainter>

WidgetBorder::WidgetBorder(QWidget *parent) : QWidget(parent), m_visible(true)
{
    setFixedSize(parent->size()); // Adjust the size as per your requirement
}

WidgetBorder::~WidgetBorder() {}

void WidgetBorder::setVisible(bool visible)
{
    m_visible = visible;
    QWidget::setVisible(visible);
}

void WidgetBorder::paintEvent(QPaintEvent *event)
{
    if (m_visible) {
        QPainter painter(this);
        QPen pen(Qt::red); // Set border color
        pen.setWidth(50); // Set border width
        painter.setPen(pen);
        painter.drawRect(rect());
    }
}


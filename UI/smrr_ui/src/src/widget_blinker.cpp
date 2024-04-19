#include "widget_blinker.h"
#include <QDebug>

WidgetBlinker::WidgetBlinker(QWidget *parent, QWidget *targetWidget)
    : QWidget(parent), targetWidget(targetWidget), isBlinkOn(false), currentToggleCount(0), toggleCount(0)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &WidgetBlinker::timer_callback);
    targetWidget->setVisible(false);
}

void WidgetBlinker::startBlinking(int delay)
{
    toggleCount = 0;
    currentToggleCount = 0;
    timer->start(delay);
}

void WidgetBlinker::stopBlinking()
{
    timer->stop();
}
void WidgetBlinker::blink(int blinks, int delay)
{
    toggleCount = blinks * 2;
    currentToggleCount = 0;
    timer->start(delay);
}

void WidgetBlinker::timer_callback()
{
    if (currentToggleCount < toggleCount || toggleCount == 0)
    {
        targetWidget->setVisible(!targetWidget->isVisible());
        currentToggleCount++;
    }
    else
    {
        timer->stop();
    }
}

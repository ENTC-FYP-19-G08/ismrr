#ifndef WIDGET_BLINKER_H
#define WIDGET_BLINKER_H

#include <QWidget>
#include <QTimer>

class WidgetBlinker : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetBlinker(QWidget *parent,QWidget *targetWidget);
    void startBlinking(int delay=500);
    void stopBlinking();
    void blink(int blinks=1,int delay=500);

private slots:
    void timer_callback();

private:
    QWidget *targetWidget;
    QTimer *timer;
    bool isBlinkOn;
    int currentToggleCount;
    int toggleCount;
};

#endif // WIDGET_BLINKER_H

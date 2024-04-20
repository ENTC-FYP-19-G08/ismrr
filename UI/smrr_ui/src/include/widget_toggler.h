#ifndef WIDGET_TOGGLER
#define WIDGET_TOGGLER

#include <QWidget>
#include <QAbstractButton>
#include <QIcon>
#include <QString>
#include "widget_blinker.h"

class WidgetToggler : public QWidget
{
    Q_OBJECT

public:
    WidgetBlinker *blinker;
    explicit WidgetToggler(QWidget *parent, QAbstractButton *targetButton, QString iconPathChecked, QString iconPathUnchecked);
    void setChecked(bool checked);

signals:
    void toggled(bool);

private slots:
    void on_clicked();

private:
    bool checked;
    QIcon iconChecked, iconUnchecked;
    QAbstractButton *targetButton;
};

#endif

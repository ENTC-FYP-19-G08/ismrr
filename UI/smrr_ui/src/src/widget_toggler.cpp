#include "widget_toggler.h"

WidgetToggler::WidgetToggler(QWidget *parent, QAbstractButton *targetButton, QString iconPathChecked, QString iconPathUnchecked)
    : QWidget(parent),
      targetButton(targetButton),
      checked(false),
      iconChecked(QIcon(iconPathChecked)),
      iconUnchecked(QIcon(iconPathUnchecked))
{
    targetButton->setIcon(iconUnchecked);
    connect(targetButton, &QAbstractButton::clicked, this, &WidgetToggler::on_clicked);
}

void WidgetToggler::on_clicked()
{    
    setChecked(!checked);
    emit toggled(checked);
}

void WidgetToggler::setChecked(bool checked){
    this->checked=checked;
    if (checked)
        targetButton->setIcon(iconChecked);
    else
        targetButton->setIcon(iconUnchecked);
}
#ifndef WIDGET_TOGGLER
#define WIDGET_TOGGLER

#include <QWidget>
#include <QAbstractButton>
#include <QIcon>
#include <QString>

class WidgetToggler : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetToggler(QWidget *parent, QAbstractButton *targetButton, QString iconPathChecked, QString iconPathUnchecked);


signals:
    void toggled(bool checked);

private slots:
    void on_clicked();

private:
    bool checked;
    QIcon iconChecked, iconUnchecked;
    QAbstractButton *targetButton;
};

#endif

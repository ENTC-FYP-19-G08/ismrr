#ifndef WIDGET_BORDER_H
#define WIDGET_BORDER_H

#include <QWidget>

class WidgetBorder : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetBorder(QWidget *parent = nullptr);
    ~WidgetBorder();

    void setVisible(bool visible) override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    bool m_visible;
};

#endif // WIDGET_BORDER_H

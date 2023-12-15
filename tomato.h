#ifndef TOMATO_H
#define TOMATO_H

#include <QWidget>
#include <QPaintEngine>

class Tomato : public QWidget
{
    Q_OBJECT

    float persent;
    QColor bg_c, ig_c, fg_c;
public:
    void setColor(QColor, QColor, QColor);
    void setPersent(float value);
    void setPersent(int value, int max);

    explicit Tomato(QWidget *parent = nullptr);


protected:
    void paintEvent(QPaintEvent *);
signals:
};

#endif // TOMATO_H

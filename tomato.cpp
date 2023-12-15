#include "tomato.h"

Tomato::Tomato(QWidget *parent)
    : QWidget{parent}
{}

void Tomato::paintEvent(QPaintEvent *)
{

    int w = this->width(), h = this->height();
    int m = h<w?h:w;

    int pensize = m/20;

    int r = (h<w?h:w)/2 - pensize;

    QPainter f_p(this), i_p(this), b_p(this);

    f_p.setPen(QPen(fg_c, pensize));
    i_p.setPen(QPen(ig_c, pensize));
    b_p.setBrush(bg_c);

    f_p.setRenderHint(QPainter::Antialiasing, true);
    i_p.setRenderHint(QPainter::Antialiasing, true);
    b_p.setRenderHint(QPainter::Antialiasing, true);

    b_p.drawRect(QRect(0, 0, w, h));

    i_p.drawArc(w/2-r, h/2-r, 2*r, 2*r, 0, 16*360);
    f_p.drawArc(w/2-r, h/2-r, 2*r, 2*r, 0, 16*persent*360);

    b_p.end();
    f_p.end();
    i_p.end();
}

void Tomato::setColor(QColor fg, QColor ig, QColor bg){
    bg_c = bg;
    ig_c = ig;
    fg_c = fg;
}

void Tomato::setPersent(float value){
    persent = value;
}

void Tomato::setPersent(int value, int max){
    persent = (float)value/max;
    if (persent > 1.0) persent = 1.0;

}

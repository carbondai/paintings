#include "timer.h"

#include <QPen>
#include <QPainter>

timer::timer(QWidget *parent)
    :QWidget(parent)
{
//    QPainter *painter;
//    painter->setRenderHint(QPainter::Antialiasing,true);
//    draw(painter);

}

void timer::draw(QPainter *painter){

    static const int triangle[3][2] = {
        {-2,-49},{2,-49},{0,-47}
    };

    QPen thickPen(palette().foreground(),1.5);
    QPen thinPen(palette().foreground(),0.5);
    QColor niceBlue(150,150,200);

    painter->setPen(thinPen);
    painter->setBrush(palette().foreground());
    painter->drawPolygon(QPolygon(3,&triangle[0][0]));
}

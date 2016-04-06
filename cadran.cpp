#include "cadran.h"

#include <QPainter>
#include <QPen>
#include <QtGui>
#include <math.h>

cadran::cadran()
{


}

void cadran::definition(int arc_width){
    arc_height = arc_width;
    arc_delta = arc_width/60;
    start_angle = 0;
    arc_length = 180;
    //arc_width = 150;
    center_x = arc_start_x + arc_width/2;
    center_y = arc_start_y + arc_height/2;
    radial_center = arc_width/10;
    radial_indicator = radial_center*2/3;

}

//void cadran::painte(QPainter painter,int arc_start_x,int arc_start_y,int arc_width,int maxVolt_plate,int maxVolt_real,double angle_indicator){
//    //QPalette palette;
//    QPen thickPen(palette().foreground(),1.5);
//    QPen thinPen(palette().foreground(),0.5);
//    QPen middlePen(palette().foreground(),2);
//    QPen indicatorPen(palette().foreground(),3);

//    QFont font_big;
//    font_big.setPixelSize(32);
//    QFont font_small;
//    font_small.setPixelSize(12);

//    definition(arc_width);
//    painter.setFont(font_small);
//    painter.setPen(middlePen);
//    painter.drawPoint(0,0);
//    painter.setRenderHint(QPainter::Antialiasing,true);
//    painter.drawArc(arc_start_x,arc_start_y,arc_width,arc_height,start_angle*16,arc_length*16);
//    for (int i=0;i<=maxVolt_plate;i++) {
//        if(i%5==0){
//            painter.setPen(thickPen);
//            painter.drawLine(center_x-cos(M_PI*i/2/15)*(arc_width/2),center_y-sin(M_PI*i/2/15)*(arc_height/2),center_x-cos(M_PI*i/2/15)*(arc_width/2-5),center_y-sin(M_PI*i/2/15)*(arc_height/2-5));
//            painter.drawText(center_x-cos(M_PI*i/2/15)*(arc_width/2-6),center_y-sin(M_PI*i/2/15)*(arc_height/2-6),15,15,Qt::AlignHCenter|Qt::AlignTop,QString::number(i));
//        }else if (i==maxVolt_real){
//            painter.setPen(QPen(Qt::red,5));
//            painter.drawLine(center_x-cos(M_PI*i/2/15)*(arc_width/2),center_y-sin(M_PI*i/2/15)*(arc_height/2),center_x-cos(M_PI*i/2/15)*(arc_width/2-5),center_y-sin(M_PI*i/2/15)*(arc_height/2-5));
//        }
//        else{
//            painter.setPen(thinPen);
//            painter.drawLine(center_x-cos(M_PI*i/2/15)*(arc_width/2),center_y-sin(M_PI*i/2/15)*(arc_height/2),center_x-cos(M_PI*i/2/15)*(arc_width/2-4),center_y-sin(M_PI*i/2/15)*(arc_height/2-4));
//        }
//    }
//    painter.setFont(font_big);
//    painter.drawText(QRectF(arc_start_x+arc_width/3,arc_start_y+arc_height/3,arc_width/3,arc_height/3),"V");
//    //indicator
//    //QPalette palette;
//    painter.setPen(thickPen);
//    painter.setBrush(QBrush(palette.brush(QPalette::Background)));
//    painter.drawEllipse(center_x-radial_center/2,center_y-radial_center/2,radial_center,radial_center);

//    painter.setBrush(Qt::black);
//    painter.drawEllipse(center_x-radial_indicator/2,center_y-radial_indicator/2,radial_indicator,radial_indicator);

//    painter.setPen(indicatorPen);
//    painter.setRenderHint(QPainter::Antialiasing,true);
//    painter.drawLine(center_x,center_y,center_x-(arc_width/2-arc_delta)*cos(angle_indicator),center_y-(arc_width/2-arc_delta)*sin(angle_indicator));


//}

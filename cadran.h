#ifndef CADRAN_H
#define CADRAN_H

#include <QPainter>

class cadran
{
public:
    cadran();

    int arc_start_x;
    int arc_start_y;
    int arc_width;
    int arc_height;
    double arc_delta;
    int start_angle;
    int arc_length;
    int center_x;
    int center_y;
    int radial_center;
    int radial_indicator;


    void definition(int);
    //void painte(QPainter,int,int,int,int,int,double);


};

#endif // CADRAN_H

#ifndef THERMOMETER_H
#define THERMOMETER_H

#include <QPointF>


class thermometer
{
public:
    thermometer();

    int temp_start_x;
    int temp_start_y;
    int temp_width;
    int temp_height;
    double delta_start_rect;

    //static const QPointF points_ther[4];//温度计刻度轮廓
    QPointF A;
    QPointF B;
    QPointF C;
    QPointF D;

    //温度计下方圆形
    double temp_center_x;
    double temp_center_y;//圆心
    double x1;
    double y1;
    double x2;
    double y2;
    double x3;
    double y3;//圆周上3个点,用来确定圆心
    double temp_radial;
    double temp_alpha;
    int temp_internal_radial;//温度计指针圆形

    void definition(int);
};

#endif // THERMOMETER_H

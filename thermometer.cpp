#include "thermometer.h"

#include <math.h>

thermometer::thermometer()
{

}

void thermometer::definition(int temp_width){
    temp_height = temp_width*4;
    delta_start_rect = temp_height*4/100;//distance from (x1,y1) to 1st -20C's scale,and some other small distance
//    //points_ther[4] = {
//    QPointF(temp_start_x+temp_width/3,temp_start_y+temp_height*8/10),
//    QPointF(temp_start_x+temp_width/3,temp_start_y+temp_height/10),
//    QPointF(temp_start_x+temp_width*2/3,temp_start_y+temp_height/10),
//    QPointF(temp_start_x+temp_width*2/3,temp_start_y+temp_height*8/10)
//};//A,B,C,D;

    A = QPointF(temp_start_x+temp_width/3,temp_start_y+temp_height*8/10);
    B = QPointF(temp_start_x+temp_width/3,temp_start_y+temp_height/10);
    C = QPointF(temp_start_x+temp_width*2/3,temp_start_y+temp_height/10);
    D = QPointF(temp_start_x+temp_width*2/3,temp_start_y+temp_height*8/10);

    temp_center_x = temp_start_x + temp_width/2;
    x1 = temp_start_x+temp_width/3;
    y1 = temp_start_y+temp_height*8/10;
    x2 = temp_start_x+temp_width*2/3;
    y2 = temp_start_y+temp_height*8/10;
    x3 = temp_center_x;
    y3 = temp_start_y+temp_height - 10;
    temp_center_y = (pow(x1-temp_center_x,2)+pow(y1,2)-pow(y3,2))/2/(y1-y3);
    temp_radial = y3-temp_center_y;
    temp_alpha = acos((pow(x1-temp_center_x,2)+pow(y1-temp_center_y,2)+pow(temp_radial,2)-pow(x1-temp_center_x,2)-pow(y1-(temp_center_y-temp_radial),2))/2/sqrt(pow(x1-temp_center_x,2)+pow(y1-temp_center_y,2))/temp_radial);

    temp_internal_radial = temp_radial*8/10;
}


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "timer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "i2c-dev.h"
#include <sys/ioctl.h>
#include "i2cbusses.h"
#include "getsensors.h"

#include <QPainter>
#include <QPen>
#include <QtGui>
#include <math.h>

#include <iostream>
#include <iomanip>
using namespace::std;

//#include "cadran.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    //resize(600,492);
    this->setFixedSize(680,320);

//    volt_12V_actual = get_sensors(1, 0x2c, 0x23) * 12.0;
//    volt_5V_actual = get_sensors(1, 0x2c, 0x22) * 3.0;
//    volt_3V_actual = get_sensors(1, 0x2c, 0x21) * 2.0;
//    speed_actual =  (int)get_sensors(1, 0x2c, 0x2f);
//    temp_actual = get_sensors(1, 0x2c, 0x1f);
//    temp_actual_2 =  get_sensors(1, 0x2c, 0x20);
    maxVolt_12V_plate = 15;
    maxVolt_12V_real = 12;
    maxVolt_5V_plate = 6;
    maxVolt_5V_real = 5.5;
    maxVolt_3V_plate = 5;
    maxVolt_3V_real = 3.5;
    maxSpeed_plate = 5000;
    maxSpeed_real = 4600;
    maxTemp_plate = 120;
    minTemp_plate = -20;
    maxTemp_real = 130;
//    angle_indicator_12V = M_PI*volt_12V_actual/maxVolt_12V_plate;
//    angle_indicator_5V = M_PI*volt_5V_actual/maxVolt_5V_plate;
//    angle_indicator_3V = M_PI*volt_3V_actual/maxVolt_3V_plate;
//    angle_indicator_speed = M_PI*speed_actual/maxSpeed_plate;
//    ui->lcdNumber_temp->display(temp_actual);
    //cout<<angle_indicator<<endl;
    ui->mainToolBar->setVisible(false);
    ui->statusBar->setVisible(false);

    QTimer *data_Timer = new QTimer(this);
    connect(data_Timer,SIGNAL(timeout()),this,SLOT(update()));
    data_Timer->start(1000);

}

void MainWindow::paintEvent(QPaintEvent *){
    QPainter painter(this);

    volt_12V_actual = get_sensors(1, 0x2c, 0x23) * 12.0;
    volt_5V_actual = get_sensors(1, 0x2c, 0x22) * 3.0;
    volt_3V_actual = get_sensors(1, 0x2c, 0x21) * 2.0;
    speed_actual =  (int)get_sensors(1, 0x2c, 0x2e);
    temp_actual = get_sensors(1, 0x2c, 0x1f);
    temp_actual_2 =  get_sensors(1, 0x2c, 0x20);
    temp_actual_3 = read_gpu("/sys/class/hwmon/hwmon0", "temp1_input");
    angle_indicator_12V = M_PI*volt_12V_actual/maxVolt_12V_plate;
    angle_indicator_5V = M_PI*volt_5V_actual/maxVolt_5V_plate;
    angle_indicator_3V = M_PI*volt_3V_actual/maxVolt_3V_plate;
    angle_indicator_speed = M_PI*speed_actual/maxSpeed_plate;

    ui->lcdNumber_volt_12V->display(volt_12V_actual);
    ui->lcdNumber_volt_5V->display(volt_5V_actual);
    ui->lcdNumber_volt_3V->display(volt_3V_actual);
    ui->lcdNumber_fan->display(speed_actual);
    ui->lcdNumber_temp->display(temp_actual);
    ui->lcdNumber_temp_2->display(temp_actual_2);
    ui->lcdNumber_temp_3->display(temp_actual_3);

    QColor lightBlue(165,192,229);
    QColor lightRed(249,190,189);
    QColor lightGreen(221,199,228);
    QColor darkBlue(53,60,68);
    QPen thickPen(palette().foreground(),1.5);
    QPen thinPen(palette().foreground(),0.5);
    QPen middlePen(palette().foreground(),2);
    QPen indicatorPen(palette().foreground(),3);

    QFont font_big;
    font_big.setPixelSize(20);
    QFont font_small;
    font_small.setPixelSize(12);
    QFont font_8;
    font_8.setPixelSize(8);

    //paint a 12V voltmeter
    volmeter_12V.arc_start_x = 20;
    volmeter_12V.arc_start_y = 20;
    volmeter_12V.arc_width = 150;
    volmeter_12V.definition(volmeter_12V.arc_width);
    painter.setFont(font_small);
    painter.setPen(middlePen);
    painter.setPen(QPen(darkBlue,3));
    painter.setRenderHint(QPainter::Antialiasing,true);
    //painter.setBrush(QBrush(lightBlue));
    painter.drawArc(volmeter_12V.arc_start_x,volmeter_12V.arc_start_y,volmeter_12V.arc_width,volmeter_12V.arc_height,volmeter_12V.start_angle*16,volmeter_12V.arc_length*16);
    painter.setPen(Qt::NoPen);
    QLinearGradient cadranGradient(volmeter_12V.center_x,volmeter_12V.center_y-volmeter_12V.arc_width/2,volmeter_12V.center_x,volmeter_12V.center_y);
    cadranGradient.setColorAt(0,lightBlue);
    cadranGradient.setColorAt(1,Qt::transparent);
    painter.setBrush(cadranGradient);
    painter.drawPie(volmeter_12V.arc_start_x,volmeter_12V.arc_start_y,volmeter_12V.arc_width,volmeter_12V.arc_height,volmeter_12V.start_angle*16,volmeter_12V.arc_length*16);

    for (int i=0;i<=maxVolt_12V_plate;i++) {
        if(i%3==0){
            painter.setPen(thickPen);
            painter.setPen(QPen(darkBlue,1.5));
            painter.drawLine(volmeter_12V.center_x-cos(M_PI*i/maxVolt_12V_plate)*(volmeter_12V.arc_width/2),volmeter_12V.center_y-sin(M_PI*i/maxVolt_12V_plate)*(volmeter_12V.arc_height/2),volmeter_12V.center_x-cos(M_PI*i/maxVolt_12V_plate)*(volmeter_12V.arc_width/2-volmeter_12V.arc_delta),volmeter_12V.center_y-sin(M_PI*i/maxVolt_12V_plate)*(volmeter_12V.arc_height/2-volmeter_12V.arc_delta));
            painter.drawText(volmeter_12V.center_x-cos(M_PI*i/maxVolt_12V_plate)*(volmeter_12V.arc_width/2-volmeter_12V.arc_delta),volmeter_12V.center_y-sin(M_PI*i/maxVolt_12V_plate)*(volmeter_12V.arc_height/2-6),15,15,Qt::AlignHCenter|Qt::AlignTop,QString::number(i));
        }else if (i==maxVolt_12V_real){
            painter.setPen(QPen(Qt::red,3));
            painter.drawLine(volmeter_12V.center_x-cos(M_PI*i/maxVolt_12V_plate)*(volmeter_12V.arc_width/2),volmeter_12V.center_y-sin(M_PI*i/maxVolt_12V_plate)*(volmeter_12V.arc_height/2),volmeter_12V.center_x-cos(M_PI*i/maxVolt_12V_plate)*(volmeter_12V.arc_width/2-volmeter_12V.arc_delta),volmeter_12V.center_y-sin(M_PI*i/maxVolt_12V_plate)*(volmeter_12V.arc_height/2-volmeter_12V.arc_delta));
        }
        else{
            painter.setPen(thinPen);
            painter.setPen(QPen(darkBlue,0.5));
            painter.drawLine(volmeter_12V.center_x-cos(M_PI*i/maxVolt_12V_plate)*(volmeter_12V.arc_width/2),volmeter_12V.center_y-sin(M_PI*i/maxVolt_12V_plate)*(volmeter_12V.arc_height/2),volmeter_12V.center_x-cos(M_PI*i/maxVolt_12V_plate)*(volmeter_12V.arc_width/2-volmeter_12V.arc_delta),volmeter_12V.center_y-sin(M_PI*i/maxVolt_12V_plate)*(volmeter_12V.arc_height/2-volmeter_12V.arc_delta));
        }
    }
    painter.setFont(font_big);
    QPalette palette;
    painter.setBrush(QBrush(palette.brush(QPalette::Background)));
    //painter.drawEllipse(25,123,25,25);
    //painter.drawText(QRectF(volmeter_12V.arc_start_x+volmeter_12V.arc_width/3,volmeter_12V.arc_start_y+volmeter_12V.arc_height/3,volmeter_12V.arc_width/3,volmeter_12V.arc_height/3),"V");
    //painter.drawText(QRectF(30,123,volmeter_12V.arc_width/3,volmeter_12V.arc_height/3),"V");
    //indicator
    //painter.setPen(thickPen);
    painter.setPen(QPen(darkBlue,1.5));
    painter.drawEllipse(volmeter_12V.center_x-volmeter_12V.radial_center/2,volmeter_12V.center_y-volmeter_12V.radial_center/2,volmeter_12V.radial_center,volmeter_12V.radial_center);
    painter.setBrush(darkBlue);
    painter.drawEllipse(volmeter_12V.center_x-volmeter_12V.radial_indicator/2,volmeter_12V.center_y-volmeter_12V.radial_indicator/2,volmeter_12V.radial_indicator,volmeter_12V.radial_indicator);

    //painter.setPen(indicatorPen);
    painter.setPen(QPen(darkBlue,3));
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.drawLine(volmeter_12V.center_x,volmeter_12V.center_y,volmeter_12V.center_x-(volmeter_12V.arc_width/2-3*volmeter_12V.arc_delta)*cos(angle_indicator_12V),volmeter_12V.center_y-(volmeter_12V.arc_width/2-3*volmeter_12V.arc_delta)*sin(angle_indicator_12V));

    //paint 5.5V volmeter
    volmeter_5V.arc_start_x = 200;
    volmeter_5V.arc_start_y = 20;
    volmeter_5V.arc_width = 150;
    volmeter_5V.definition(volmeter_5V.arc_width);
    painter.setFont(font_small);
    painter.setPen(middlePen);

    painter.drawArc(volmeter_5V.arc_start_x,volmeter_5V.arc_start_y,volmeter_5V.arc_width,volmeter_5V.arc_height,volmeter_5V.start_angle*16,volmeter_5V.arc_length*16);
    painter.setPen(Qt::NoPen);
    QLinearGradient cadranGradient_5V(volmeter_5V.center_x,volmeter_5V.center_y-volmeter_5V.arc_width/2,volmeter_5V.center_x,volmeter_5V.center_y);
    cadranGradient_5V.setColorAt(0,lightBlue);
    cadranGradient_5V.setColorAt(1,Qt::transparent);
    painter.setBrush(cadranGradient_5V);
    painter.drawPie(volmeter_5V.arc_start_x,volmeter_5V.arc_start_y,volmeter_5V.arc_width,volmeter_5V.arc_height,volmeter_5V.start_angle*16,volmeter_5V.arc_length*16);

    int k = 10;
    for (int i=0;i<=maxVolt_5V_plate*k;i++) {
        //cout<<i<<endl;
        if(i%k==0){
            painter.setPen(thickPen);
            painter.drawLine(volmeter_5V.center_x-cos(M_PI*i/k/maxVolt_5V_plate)*(volmeter_5V.arc_width/2),volmeter_5V.center_y-sin(M_PI*i/k/maxVolt_5V_plate)*(volmeter_5V.arc_height/2),volmeter_5V.center_x-cos(M_PI*i/k/maxVolt_5V_plate)*(volmeter_5V.arc_width/2-volmeter_5V.arc_delta),volmeter_5V.center_y-sin(M_PI*i/k/maxVolt_5V_plate)*(volmeter_5V.arc_height/2-volmeter_5V.arc_delta));
            painter.drawText(volmeter_5V.center_x-cos(M_PI*i/k/maxVolt_5V_plate)*(volmeter_5V.arc_width/2-volmeter_5V.arc_delta),volmeter_5V.center_y-sin(M_PI*i/k/maxVolt_5V_plate)*(volmeter_5V.arc_height/2-volmeter_5V.arc_delta),15,15,Qt::AlignHCenter|Qt::AlignTop,QString::number(i/k));
        }else if (i==maxVolt_5V_real*k){
            //cout<<i<<endl;
            painter.setPen(QPen(Qt::red,3));
            painter.drawLine(volmeter_5V.center_x-cos(M_PI*i/k/maxVolt_5V_plate)*(volmeter_5V.arc_width/2),volmeter_5V.center_y-sin(M_PI*i/k/maxVolt_5V_plate)*(volmeter_5V.arc_height/2),volmeter_5V.center_x-cos(M_PI*i/k/maxVolt_5V_plate)*(volmeter_5V.arc_width/2-volmeter_5V.arc_delta),volmeter_5V.center_y-sin(M_PI*i/k/maxVolt_5V_plate)*(volmeter_5V.arc_height/2-volmeter_5V.arc_delta));
        }
        else{
            painter.setPen(thinPen);
            painter.drawLine(volmeter_5V.center_x-cos(M_PI*i/k/maxVolt_5V_plate)*(volmeter_5V.arc_width/2),volmeter_5V.center_y-sin(M_PI*i/k/maxVolt_5V_plate)*(volmeter_5V.arc_height/2),volmeter_5V.center_x-cos(M_PI*i/k/maxVolt_5V_plate)*(volmeter_5V.arc_width/2-volmeter_5V.arc_delta),volmeter_5V.center_y-sin(M_PI*i/k/maxVolt_5V_plate)*(volmeter_5V.arc_height/2-volmeter_5V.arc_delta));
        }
    }
    painter.setFont(font_big);
    painter.drawText(QRectF(volmeter_5V.arc_start_x+volmeter_5V.arc_width/3,volmeter_5V.arc_start_y+volmeter_5V.arc_height/3,volmeter_5V.arc_width/3,volmeter_5V.arc_height/3),"V");
    //indicator
    painter.setPen(thickPen);
    painter.setBrush(QBrush(palette.brush(QPalette::Background)));
    painter.drawEllipse(volmeter_5V.center_x-volmeter_5V.radial_center/2,volmeter_5V.center_y-volmeter_5V.radial_center/2,volmeter_5V.radial_center,volmeter_5V.radial_center);

    painter.setBrush(Qt::black);
    painter.drawEllipse(volmeter_5V.center_x-volmeter_5V.radial_indicator/2,volmeter_5V.center_y-volmeter_5V.radial_indicator/2,volmeter_5V.radial_indicator,volmeter_5V.radial_indicator);

    painter.setPen(indicatorPen);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.drawLine(volmeter_5V.center_x,volmeter_5V.center_y,volmeter_5V.center_x-(volmeter_5V.arc_width/2-3*volmeter_5V.arc_delta)*cos(angle_indicator_5V),volmeter_5V.center_y-(volmeter_5V.arc_width/2-3*volmeter_5V.arc_delta)*sin(angle_indicator_5V));

    //paint 3V volmeter
    volmeter_3V.arc_start_x = 20;
    volmeter_3V.arc_start_y = 180;
    volmeter_3V.arc_width = 150;
    volmeter_3V.definition(volmeter_3V.arc_width);
    painter.setFont(font_small);
    painter.setPen(middlePen);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.drawArc(volmeter_3V.arc_start_x,volmeter_3V.arc_start_y,volmeter_3V.arc_width,volmeter_3V.arc_height,volmeter_3V.start_angle*16,volmeter_3V.arc_length*16);
    painter.setPen(Qt::NoPen);
    QLinearGradient cadranGradient_3V(volmeter_3V.center_x,volmeter_3V.center_y-volmeter_3V.arc_width/2,volmeter_3V.center_x,volmeter_3V.center_y);
    cadranGradient_3V.setColorAt(0,lightBlue);
    cadranGradient_3V.setColorAt(1,Qt::transparent);
    painter.setBrush(cadranGradient_3V);
    painter.drawPie(volmeter_3V.arc_start_x,volmeter_3V.arc_start_y,volmeter_3V.arc_width,volmeter_3V.arc_height,volmeter_3V.start_angle*16,volmeter_3V.arc_length*16);

    for (int i=0;i<=maxVolt_3V_plate*k;i++) {
        //cout<<i<<endl;
        if(i%k==0){
            painter.setPen(thickPen);
            painter.drawLine(volmeter_3V.center_x-cos(M_PI*i/k/maxVolt_3V_plate)*(volmeter_3V.arc_width/2),volmeter_3V.center_y-sin(M_PI*i/k/maxVolt_3V_plate)*(volmeter_3V.arc_height/2),volmeter_3V.center_x-cos(M_PI*i/k/maxVolt_3V_plate)*(volmeter_3V.arc_width/2-volmeter_3V.arc_delta),volmeter_3V.center_y-sin(M_PI*i/k/maxVolt_3V_plate)*(volmeter_3V.arc_height/2-volmeter_3V.arc_delta));
            painter.drawText(volmeter_3V.center_x-cos(M_PI*i/k/maxVolt_3V_plate)*(volmeter_3V.arc_width/2-volmeter_3V.arc_delta),volmeter_3V.center_y-sin(M_PI*i/k/maxVolt_3V_plate)*(volmeter_3V.arc_height/2-volmeter_3V.arc_delta),15,15,Qt::AlignHCenter|Qt::AlignTop,QString::number(i/k));
        }else if (i==maxVolt_3V_real*k){
            painter.setPen(QPen(Qt::red,3));
            painter.drawLine(volmeter_3V.center_x-cos(M_PI*i/k/maxVolt_3V_plate)*(volmeter_3V.arc_width/2),volmeter_3V.center_y-sin(M_PI*i/k/maxVolt_3V_plate)*(volmeter_3V.arc_height/2),volmeter_3V.center_x-cos(M_PI*i/k/maxVolt_3V_plate)*(volmeter_3V.arc_width/2-volmeter_3V.arc_delta),volmeter_3V.center_y-sin(M_PI*i/k/maxVolt_3V_plate)*(volmeter_3V.arc_height/2-volmeter_3V.arc_delta));
        }
        else{
            painter.setPen(thinPen);
            painter.drawLine(volmeter_3V.center_x-cos(M_PI*i/k/maxVolt_3V_plate)*(volmeter_3V.arc_width/2),volmeter_3V.center_y-sin(M_PI*i/k/maxVolt_3V_plate)*(volmeter_3V.arc_height/2),volmeter_3V.center_x-cos(M_PI*i/k/maxVolt_3V_plate)*(volmeter_3V.arc_width/2-volmeter_3V.arc_delta),volmeter_3V.center_y-sin(M_PI*i/k/maxVolt_3V_plate)*(volmeter_3V.arc_height/2-volmeter_3V.arc_delta));
        }
    }
    painter.setFont(font_big);
    painter.drawText(QRectF(volmeter_3V.arc_start_x+volmeter_3V.arc_width/3,volmeter_3V.arc_start_y+volmeter_3V.arc_height/3,volmeter_3V.arc_width/3,volmeter_3V.arc_height/3),"V");
    //indicator
    //QPalette palette;
    painter.setPen(thickPen);
    painter.setBrush(QBrush(palette.brush(QPalette::Background)));
    painter.drawEllipse(volmeter_3V.center_x-volmeter_3V.radial_center/2,volmeter_3V.center_y-volmeter_3V.radial_center/2,volmeter_3V.radial_center,volmeter_3V.radial_center);

    painter.setBrush(Qt::black);
    painter.drawEllipse(volmeter_3V.center_x-volmeter_3V.radial_indicator/2,volmeter_3V.center_y-volmeter_3V.radial_indicator/2,volmeter_3V.radial_indicator,volmeter_3V.radial_indicator);

    painter.setPen(indicatorPen);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.drawLine(volmeter_3V.center_x,volmeter_3V.center_y,volmeter_3V.center_x-(volmeter_3V.arc_width/2-3*volmeter_3V.arc_delta)*cos(angle_indicator_3V),volmeter_3V.center_y-(volmeter_3V.arc_width/2-3*volmeter_3V.arc_delta)*sin(angle_indicator_3V));

    //paint fan speed cadran
    fan_speed.arc_start_x = 200;
    fan_speed.arc_start_y = 180;
    fan_speed.arc_width = 150;
    fan_speed.definition(fan_speed.arc_width);
    painter.setFont(font_small);
    painter.setPen(middlePen);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.drawArc(fan_speed.arc_start_x,fan_speed.arc_start_y,fan_speed.arc_width,fan_speed.arc_height,fan_speed.start_angle*16,fan_speed.arc_length*16);
    painter.setPen(Qt::NoPen);
    QLinearGradient cadranGradient_speed(fan_speed.center_x,fan_speed.center_y-fan_speed.arc_width/2,fan_speed.center_x,fan_speed.center_y);
    cadranGradient_speed.setColorAt(0,lightRed);
    cadranGradient_speed.setColorAt(1,Qt::transparent);
    painter.setBrush(cadranGradient_speed);
    painter.drawPie(fan_speed.arc_start_x,fan_speed.arc_start_y,fan_speed.arc_width,fan_speed.arc_height,fan_speed.start_angle*16,fan_speed.arc_length*16);

    int k_fan = 1;
    for (int i=0;i<=maxSpeed_plate*k_fan;i++) {
        //cout<<i<<endl;
        if(i%1000==0){
            painter.setPen(thickPen);
            painter.drawLine(fan_speed.center_x-cos(M_PI*i/k_fan/maxSpeed_plate)*(fan_speed.arc_width/2),fan_speed.center_y-sin(M_PI*i/k_fan/maxSpeed_plate)*(fan_speed.arc_height/2),fan_speed.center_x-cos(M_PI*i/k_fan/maxSpeed_plate)*(fan_speed.arc_width/2-fan_speed.arc_delta),fan_speed.center_y-sin(M_PI*i/k_fan/maxSpeed_plate)*(fan_speed.arc_height/2-fan_speed.arc_delta));
            painter.drawText(fan_speed.center_x-cos(M_PI*i/k_fan/maxSpeed_plate)*(fan_speed.arc_width/2-fan_speed.arc_delta),fan_speed.center_y-sin(M_PI*i/k_fan/maxSpeed_plate)*(fan_speed.arc_height/2-fan_speed.arc_delta),30,30,Qt::AlignHCenter|Qt::AlignTop,QString::number(i/k_fan));
        }else if (i==maxSpeed_real*k_fan){
            //cout<<i<<endl;
            painter.setPen(QPen(Qt::red,5));
            painter.drawLine(fan_speed.center_x-cos(M_PI*i/k_fan/maxSpeed_plate)*(fan_speed.arc_width/2),fan_speed.center_y-sin(M_PI*i/k_fan/maxSpeed_plate)*(fan_speed.arc_height/2),fan_speed.center_x-cos(M_PI*i/k_fan/maxSpeed_plate)*(fan_speed.arc_width/2-fan_speed.arc_delta),fan_speed.center_y-sin(M_PI*i/k_fan/maxSpeed_plate)*(fan_speed.arc_height/2-fan_speed.arc_delta));
        }
        else if (i%100==0){
            painter.setPen(thinPen);
            painter.drawLine(fan_speed.center_x-cos(M_PI*i/k_fan/maxSpeed_plate)*(fan_speed.arc_width/2),fan_speed.center_y-sin(M_PI*i/k_fan/maxSpeed_plate)*(fan_speed.arc_height/2),fan_speed.center_x-cos(M_PI*i/k_fan/maxSpeed_plate)*(fan_speed.arc_width/2-fan_speed.arc_delta),fan_speed.center_y-sin(M_PI*i/k_fan/maxSpeed_plate)*(fan_speed.arc_height/2-fan_speed.arc_delta));
        }
    }
    painter.setFont(font_big);
    painter.drawText(QRectF(fan_speed.arc_start_x+fan_speed.arc_width/3,fan_speed.arc_start_y+fan_speed.arc_height/3,fan_speed.arc_width/3,fan_speed.arc_height/3),"rad");
    //indicator
    painter.setPen(thickPen);
    painter.setBrush(QBrush(palette.brush(QPalette::Background)));
    painter.drawEllipse(fan_speed.center_x-fan_speed.radial_center/2,fan_speed.center_y-fan_speed.radial_center/2,fan_speed.radial_center,fan_speed.radial_center);

    painter.setBrush(Qt::black);
    painter.drawEllipse(fan_speed.center_x-fan_speed.radial_indicator/2,fan_speed.center_y-fan_speed.radial_indicator/2,fan_speed.radial_indicator,fan_speed.radial_indicator);

    painter.setPen(indicatorPen);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.drawLine(fan_speed.center_x,fan_speed.center_y,fan_speed.center_x-(fan_speed.arc_width/2-3*fan_speed.arc_delta)*cos(angle_indicator_speed),fan_speed.center_y-(fan_speed.arc_width/2-3*fan_speed.arc_delta)*sin(angle_indicator_speed));


    //paint thermometer 1
    painter.setFont(font_small);
    ther_1.temp_start_x = 420;
    ther_1.temp_start_y = 20;
    ther_1.temp_width = 60;
    ther_1.definition(ther_1.temp_width);

    painter.setPen(indicatorPen);
    painter.setRenderHint(QPainter::Antialiasing,true);
    QLinearGradient cadranGradient_ther1(ther_1.temp_start_x,ther_1.temp_start_y,ther_1.temp_start_x,ther_1.temp_start_y+ther_1.temp_height);
    cadranGradient_ther1.setColorAt(0,lightGreen);
    cadranGradient_ther1.setColorAt(1,Qt::transparent);
    painter.setBrush(cadranGradient_ther1);
    painter.drawRoundRect(ther_1.temp_start_x,ther_1.temp_start_y,ther_1.temp_width,ther_1.temp_height,25,25);

    static const QPointF points_ther[4] = {ther_1.A,ther_1.B,ther_1.C,ther_1.D};

    painter.setPen(thickPen);
    painter.drawPolyline(points_ther,4);
    painter.drawArc(ther_1.temp_center_x-ther_1.temp_radial,ther_1.temp_center_y-ther_1.temp_radial,ther_1.temp_radial*2,ther_1.temp_radial*2,(ther_1.temp_alpha*180/M_PI+90)*16,(360-ther_1.temp_alpha*2*180/M_PI)*16);


 //   double delta_y = (temp_start_y+temp_height*8/10 - (temp_start_y+temp_height/10) - 10)/(maxTemp_plate-minTemp_plate);
    QImage img_C;
    img_C.load("./C.png");
    QRect C_rect(ther_1.temp_start_x+10,ther_1.temp_start_y,ther_1.temp_width/5,ther_1.temp_width/5);
    QImage img_F;
    img_F.load("./F.png");
    QRect F_rect(ther_1.temp_start_x+ther_1.temp_width-ther_1.temp_width/5-5,ther_1.temp_start_y,ther_1.temp_width/5,ther_1.temp_width/5);
    painter.drawImage(F_rect,img_F);
    painter.drawImage(C_rect,img_C);
    for (int i=0;i<=(maxTemp_plate-minTemp_plate);i++) {
        if(i%20==0){
            //cout<<i<<endl;
            painter.setPen(thickPen);
            painter.drawLine(ther_1.x1, ther_1.temp_start_y+ther_1.temp_height*8/10 - (ther_1.temp_start_y+ther_1.temp_height*8/10 - (ther_1.temp_start_y+ther_1.temp_height/10)-ther_1.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_1.delta_start_rect,ther_1.x1-ther_1.delta_start_rect/2,ther_1.temp_start_y+ther_1.temp_height*8/10 - (ther_1.temp_start_y+ther_1.temp_height*8/10 - (ther_1.temp_start_y+ther_1.temp_height/10)-ther_1.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_1.delta_start_rect);
            painter.drawLine(ther_1.x2, ther_1.temp_start_y+ther_1.temp_height*8/10 - (ther_1.temp_start_y+ther_1.temp_height*8/10 - (ther_1.temp_start_y+ther_1.temp_height/10)-ther_1.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_1.delta_start_rect,ther_1.x2+ther_1.delta_start_rect/2,ther_1.temp_start_y+ther_1.temp_height*8/10 - (ther_1.temp_start_y+ther_1.temp_height*8/10 - (ther_1.temp_start_y+ther_1.temp_height/10)-ther_1.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_1.delta_start_rect);
            painter.setFont(font_8);
            painter.drawText(ther_1.x1-ther_1.delta_start_rect*2.5,-ther_1.delta_start_rect/2+ther_1.temp_start_y+ther_1.temp_height*8/10 - (ther_1.temp_start_y+ther_1.temp_height*8/10 - (ther_1.temp_start_y+ther_1.temp_height/10) - ther_1.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_1.delta_start_rect,25,15,Qt::AlignHCenter|Qt::AlignTop,QString::number(i+minTemp_plate));
            painter.drawText(ther_1.x2+ther_1.delta_start_rect/10,-ther_1.delta_start_rect/2+ther_1.temp_start_y+ther_1.temp_height*8/10 - (ther_1.temp_start_y+ther_1.temp_height*8/10 - (ther_1.temp_start_y+ther_1.temp_height/10) - ther_1.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_1.delta_start_rect,25,15,Qt::AlignHCenter|Qt::AlignTop,QString::number((i+minTemp_plate)*1.8+32));

        }else if (i==(maxTemp_real-minTemp_plate)){
            //cout<<maxTemp_real<<endl;
            painter.setPen(QPen(Qt::red,3));
            painter.drawLine(ther_1.x1, ther_1.temp_start_y+ther_1.temp_height*8/10 - (ther_1.temp_start_y+ther_1.temp_height*8/10 - (ther_1.temp_start_y+ther_1.temp_height/10) - ther_1.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_1.delta_start_rect,ther_1.x1-ther_1.delta_start_rect/2,ther_1.temp_start_y+ther_1.temp_height*8/10 - (ther_1.temp_start_y+ther_1.temp_height*8/10 - (ther_1.temp_start_y+ther_1.temp_height/10) - ther_1.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_1.delta_start_rect);
            painter.drawLine(ther_1.x2, ther_1.temp_start_y+ther_1.temp_height*8/10 - (ther_1.temp_start_y+ther_1.temp_height*8/10 - (ther_1.temp_start_y+ther_1.temp_height/10) - ther_1.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_1.delta_start_rect,ther_1.x2+ther_1.delta_start_rect/2,ther_1.temp_start_y+ther_1.temp_height*8/10 - (ther_1.temp_start_y+ther_1.temp_height*8/10 - (ther_1.temp_start_y+ther_1.temp_height/10) - ther_1.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_1.delta_start_rect);
        }
        else if (i%5==0){
            painter.setPen(thinPen);
            painter.drawLine(ther_1.x1, ther_1.temp_start_y+ther_1.temp_height*8/10 - (ther_1.temp_start_y+ther_1.temp_height*8/10 - (ther_1.temp_start_y+ther_1.temp_height/10) - ther_1.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_1.delta_start_rect,ther_1.x1-ther_1.delta_start_rect/2,ther_1.temp_start_y+ther_1.temp_height*8/10 - (ther_1.temp_start_y+ther_1.temp_height*8/10 - (ther_1.temp_start_y+ther_1.temp_height/10) - ther_1.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_1.delta_start_rect);
            painter.drawLine(ther_1.x2, ther_1.temp_start_y+ther_1.temp_height*8/10 - (ther_1.temp_start_y+ther_1.temp_height*8/10 - (ther_1.temp_start_y+ther_1.temp_height/10)-ther_1.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_1.delta_start_rect,ther_1.x2+ther_1.delta_start_rect/2,ther_1.temp_start_y+ther_1.temp_height*8/10 - (ther_1.temp_start_y+ther_1.temp_height*8/10 - (ther_1.temp_start_y+ther_1.temp_height/10)-ther_1.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_1.delta_start_rect);
        }
    }

    //paint actual temperature
    painter.setBrush(QBrush(Qt::black));
    painter.drawEllipse(ther_1.temp_center_x-ther_1.temp_internal_radial,ther_1.temp_center_y-ther_1.temp_internal_radial,ther_1.temp_internal_radial*2,ther_1.temp_internal_radial*2);
    painter.drawRect(ther_1.x1+(ther_1.x2-ther_1.x1)*2/10,ther_1.y1,(ther_1.x2-ther_1.x1)*6/10,-ther_1.delta_start_rect-(ther_1.temp_start_y+ther_1.temp_height*8/10 - (ther_1.temp_start_y+ther_1.temp_height/10) -ther_1.delta_start_rect)*(temp_actual-minTemp_plate)/(maxTemp_plate-minTemp_plate));

    //paint thermometer2
    painter.setFont(font_small);
    ther_2.temp_start_x = 495;
    ther_2.temp_start_y = 20;
    ther_2.temp_width = 60;
    ther_2.definition(ther_2.temp_width);

    painter.setPen(indicatorPen);
    painter.setRenderHint(QPainter::Antialiasing,true);
    QLinearGradient cadranGradient_ther2(ther_2.temp_start_x,ther_2.temp_start_y,ther_2.temp_start_x,ther_2.temp_start_y+ther_2.temp_height);
    cadranGradient_ther2.setColorAt(0,lightGreen);
    cadranGradient_ther2.setColorAt(1,Qt::transparent);
    painter.setBrush(cadranGradient_ther2);
    painter.drawRoundRect(ther_2.temp_start_x,ther_2.temp_start_y,ther_2.temp_width,ther_2.temp_height,25,25);

    static const QPointF points_ther_2[4] = {ther_2.A,ther_2.B,ther_2.C,ther_2.D};

    painter.setPen(thickPen);
    painter.drawPolyline(points_ther_2,4);
    painter.drawArc(ther_2.temp_center_x-ther_2.temp_radial,ther_2.temp_center_y-ther_2.temp_radial,ther_2.temp_radial*2,ther_2.temp_radial*2,(ther_2.temp_alpha*180/M_PI+90)*16,(360-ther_2.temp_alpha*2*180/M_PI)*16);


 //   double delta_y = (temp_start_y+temp_height*8/10 - (temp_start_y+temp_height/10) - 10)/(maxTemp_plate-minTemp_plate);
    QRect C_rect_2(ther_2.temp_start_x+10,ther_2.temp_start_y,ther_2.temp_width/5,ther_2.temp_width/5);
    QRect F_rect_2(ther_2.temp_start_x+ther_2.temp_width-ther_2.temp_width/5-5,ther_2.temp_start_y,ther_2.temp_width/5,ther_2.temp_width/5);
    painter.drawImage(F_rect_2,img_F);
    painter.drawImage(C_rect_2,img_C);
    for (int i=0;i<=(maxTemp_plate-minTemp_plate);i++) {
        if(i%20==0){
            //cout<<i<<endl;
            painter.setPen(thickPen);
            painter.drawLine(ther_2.x1, ther_2.temp_start_y+ther_2.temp_height*8/10 - (ther_2.temp_start_y+ther_2.temp_height*8/10 - (ther_2.temp_start_y+ther_2.temp_height/10)-ther_2.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_2.delta_start_rect,ther_2.x1-ther_2.delta_start_rect/2,ther_2.temp_start_y+ther_2.temp_height*8/10 - (ther_2.temp_start_y+ther_2.temp_height*8/10 - (ther_2.temp_start_y+ther_2.temp_height/10)-ther_2.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_2.delta_start_rect);
            painter.drawLine(ther_2.x2, ther_2.temp_start_y+ther_2.temp_height*8/10 - (ther_2.temp_start_y+ther_2.temp_height*8/10 - (ther_2.temp_start_y+ther_2.temp_height/10)-ther_2.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_2.delta_start_rect,ther_2.x2+ther_2.delta_start_rect/2,ther_2.temp_start_y+ther_2.temp_height*8/10 - (ther_2.temp_start_y+ther_2.temp_height*8/10 - (ther_2.temp_start_y+ther_2.temp_height/10)-ther_2.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_2.delta_start_rect);
            painter.setFont(font_8);
            painter.drawText(ther_2.x1-ther_2.delta_start_rect*2.5,-ther_2.delta_start_rect/2+ther_2.temp_start_y+ther_2.temp_height*8/10 - (ther_2.temp_start_y+ther_2.temp_height*8/10 - (ther_2.temp_start_y+ther_2.temp_height/10) - ther_2.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_2.delta_start_rect,25,15,Qt::AlignHCenter|Qt::AlignTop,QString::number(i+minTemp_plate));
            painter.drawText(ther_2.x2+ther_2.delta_start_rect/10,-ther_2.delta_start_rect/2+ther_2.temp_start_y+ther_2.temp_height*8/10 - (ther_2.temp_start_y+ther_2.temp_height*8/10 - (ther_2.temp_start_y+ther_2.temp_height/10) - ther_2.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_2.delta_start_rect,25,15,Qt::AlignHCenter|Qt::AlignTop,QString::number((i+minTemp_plate)*1.8+32));

        }else if (i==(maxTemp_real-minTemp_plate)){
            //cout<<maxTemp_real<<endl;
            painter.setPen(QPen(Qt::red,3));
            painter.drawLine(ther_2.x1, ther_2.temp_start_y+ther_2.temp_height*8/10 - (ther_2.temp_start_y+ther_2.temp_height*8/10 - (ther_2.temp_start_y+ther_2.temp_height/10) - ther_2.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_2.delta_start_rect,ther_2.x1-ther_2.delta_start_rect/2,ther_2.temp_start_y+ther_2.temp_height*8/10 - (ther_2.temp_start_y+ther_2.temp_height*8/10 - (ther_2.temp_start_y+ther_2.temp_height/10) - ther_2.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_2.delta_start_rect);
            painter.drawLine(ther_2.x2, ther_2.temp_start_y+ther_2.temp_height*8/10 - (ther_2.temp_start_y+ther_2.temp_height*8/10 - (ther_2.temp_start_y+ther_2.temp_height/10) - ther_2.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_2.delta_start_rect,ther_2.x2+ther_2.delta_start_rect/2,ther_2.temp_start_y+ther_2.temp_height*8/10 - (ther_2.temp_start_y+ther_2.temp_height*8/10 - (ther_2.temp_start_y+ther_2.temp_height/10) - ther_2.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_2.delta_start_rect);
        }
        else if (i%5==0){
            painter.setPen(thinPen);
            painter.drawLine(ther_2.x1, ther_2.temp_start_y+ther_2.temp_height*8/10 - (ther_2.temp_start_y+ther_2.temp_height*8/10 - (ther_2.temp_start_y+ther_2.temp_height/10) - ther_2.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_2.delta_start_rect,ther_2.x1-ther_2.delta_start_rect/2,ther_2.temp_start_y+ther_2.temp_height*8/10 - (ther_2.temp_start_y+ther_2.temp_height*8/10 - (ther_2.temp_start_y+ther_2.temp_height/10) - ther_2.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_2.delta_start_rect);
            painter.drawLine(ther_2.x2, ther_2.temp_start_y+ther_2.temp_height*8/10 - (ther_2.temp_start_y+ther_2.temp_height*8/10 - (ther_2.temp_start_y+ther_2.temp_height/10)-ther_2.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_2.delta_start_rect,ther_2.x2+ther_2.delta_start_rect/2,ther_2.temp_start_y+ther_2.temp_height*8/10 - (ther_2.temp_start_y+ther_2.temp_height*8/10 - (ther_2.temp_start_y+ther_2.temp_height/10)-ther_2.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_2.delta_start_rect);
        }
    }

    //paint actual temperature
    painter.setBrush(QBrush(Qt::black));
    painter.drawEllipse(ther_2.temp_center_x-ther_2.temp_internal_radial,ther_2.temp_center_y-ther_2.temp_internal_radial,ther_2.temp_internal_radial*2,ther_2.temp_internal_radial*2);
    painter.drawRect(ther_2.x1+(ther_2.x2-ther_2.x1)*2/10,ther_2.y1,(ther_2.x2-ther_2.x1)*6/10,-ther_2.delta_start_rect-(ther_2.temp_start_y+ther_2.temp_height*8/10 - (ther_2.temp_start_y+ther_2.temp_height/10) -ther_2.delta_start_rect)*(temp_actual_2-minTemp_plate)/(maxTemp_plate-minTemp_plate));

    //paint thermometer3
    painter.setFont(font_small);
    ther_3.temp_start_x = 570;
    ther_3.temp_start_y = 20;
    ther_3.temp_width = 60;
    ther_3.definition(ther_3.temp_width);

    painter.setPen(indicatorPen);
    painter.setRenderHint(QPainter::Antialiasing,true);
    QLinearGradient cadranGradient_ther3(ther_3.temp_start_x,ther_3.temp_start_y,ther_3.temp_start_x,ther_3.temp_start_y+ther_3.temp_height);
    cadranGradient_ther3.setColorAt(0,lightGreen);
    cadranGradient_ther3.setColorAt(1,Qt::transparent);
    painter.setBrush(cadranGradient_ther3);
    painter.drawRoundRect(ther_3.temp_start_x,ther_3.temp_start_y,ther_3.temp_width,ther_3.temp_height,25,25);

    static const QPointF points_ther_3[4] = {ther_3.A,ther_3.B,ther_3.C,ther_3.D};

    painter.setPen(thickPen);
    painter.drawPolyline(points_ther_3,4);
    painter.drawArc(ther_3.temp_center_x-ther_3.temp_radial,ther_3.temp_center_y-ther_3.temp_radial,ther_3.temp_radial*2,ther_3.temp_radial*2,(ther_3.temp_alpha*180/M_PI+90)*16,(360-ther_3.temp_alpha*2*180/M_PI)*16);


 //   double delta_y = (temp_start_y+temp_height*8/10 - (temp_start_y+temp_height/10) - 10)/(maxTemp_plate-minTemp_plate);
    QRect C_rect_3(ther_3.temp_start_x+10,ther_3.temp_start_y,ther_3.temp_width/5,ther_3.temp_width/5);
    QRect F_rect_3(ther_3.temp_start_x+ther_3.temp_width-ther_3.temp_width/5-5,ther_3.temp_start_y,ther_3.temp_width/5,ther_3.temp_width/5);
    painter.drawImage(F_rect_3,img_F);
    painter.drawImage(C_rect_3,img_C);
    for (int i=0;i<=(maxTemp_plate-minTemp_plate);i++) {
        if(i%20==0){
            //cout<<i<<endl;
            painter.setPen(thickPen);
            painter.drawLine(ther_3.x1, ther_3.temp_start_y+ther_3.temp_height*8/10 - (ther_3.temp_start_y+ther_3.temp_height*8/10 - (ther_3.temp_start_y+ther_3.temp_height/10)-ther_3.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_3.delta_start_rect,ther_3.x1-ther_3.delta_start_rect/2,ther_3.temp_start_y+ther_3.temp_height*8/10 - (ther_3.temp_start_y+ther_3.temp_height*8/10 - (ther_3.temp_start_y+ther_3.temp_height/10)-ther_3.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_3.delta_start_rect);
            painter.drawLine(ther_3.x2, ther_3.temp_start_y+ther_3.temp_height*8/10 - (ther_3.temp_start_y+ther_3.temp_height*8/10 - (ther_3.temp_start_y+ther_3.temp_height/10)-ther_3.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_3.delta_start_rect,ther_3.x2+ther_3.delta_start_rect/2,ther_3.temp_start_y+ther_3.temp_height*8/10 - (ther_3.temp_start_y+ther_3.temp_height*8/10 - (ther_3.temp_start_y+ther_3.temp_height/10)-ther_3.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_3.delta_start_rect);
            painter.setFont(font_8);
            painter.drawText(ther_3.x1-ther_3.delta_start_rect*2.5,-ther_3.delta_start_rect/2+ther_3.temp_start_y+ther_3.temp_height*8/10 - (ther_3.temp_start_y+ther_3.temp_height*8/10 - (ther_3.temp_start_y+ther_3.temp_height/10) - ther_3.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_3.delta_start_rect,25,15,Qt::AlignHCenter|Qt::AlignTop,QString::number(i+minTemp_plate));
            painter.drawText(ther_3.x2+ther_3.delta_start_rect/10,-ther_3.delta_start_rect/2+ther_3.temp_start_y+ther_3.temp_height*8/10 - (ther_3.temp_start_y+ther_3.temp_height*8/10 - (ther_3.temp_start_y+ther_3.temp_height/10) - ther_3.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_3.delta_start_rect,25,15,Qt::AlignHCenter|Qt::AlignTop,QString::number((i+minTemp_plate)*1.8+32));

        }else if (i==(maxTemp_real-minTemp_plate)){
            //cout<<maxTemp_real<<endl;
            painter.setPen(QPen(Qt::red,3));
            painter.drawLine(ther_3.x1, ther_3.temp_start_y+ther_3.temp_height*8/10 - (ther_3.temp_start_y+ther_3.temp_height*8/10 - (ther_3.temp_start_y+ther_3.temp_height/10) - ther_3.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_3.delta_start_rect,ther_3.x1-ther_3.delta_start_rect/2,ther_3.temp_start_y+ther_3.temp_height*8/10 - (ther_3.temp_start_y+ther_3.temp_height*8/10 - (ther_3.temp_start_y+ther_3.temp_height/10) - ther_3.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_3.delta_start_rect);
            painter.drawLine(ther_3.x2, ther_3.temp_start_y+ther_3.temp_height*8/10 - (ther_3.temp_start_y+ther_3.temp_height*8/10 - (ther_3.temp_start_y+ther_3.temp_height/10) - ther_3.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_3.delta_start_rect,ther_3.x2+ther_3.delta_start_rect/2,ther_3.temp_start_y+ther_3.temp_height*8/10 - (ther_3.temp_start_y+ther_3.temp_height*8/10 - (ther_3.temp_start_y+ther_3.temp_height/10) - ther_3.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_3.delta_start_rect);
        }
        else if (i%5==0){
            painter.setPen(thinPen);
            painter.drawLine(ther_3.x1, ther_3.temp_start_y+ther_3.temp_height*8/10 - (ther_3.temp_start_y+ther_3.temp_height*8/10 - (ther_3.temp_start_y+ther_3.temp_height/10) - ther_3.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_3.delta_start_rect,ther_3.x1-ther_3.delta_start_rect/2,ther_3.temp_start_y+ther_3.temp_height*8/10 - (ther_3.temp_start_y+ther_3.temp_height*8/10 - (ther_3.temp_start_y+ther_3.temp_height/10) - ther_3.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_3.delta_start_rect);
            painter.drawLine(ther_3.x2, ther_3.temp_start_y+ther_3.temp_height*8/10 - (ther_3.temp_start_y+ther_3.temp_height*8/10 - (ther_3.temp_start_y+ther_3.temp_height/10)-ther_3.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_3.delta_start_rect,ther_3.x2+ther_3.delta_start_rect/2,ther_3.temp_start_y+ther_3.temp_height*8/10 - (ther_3.temp_start_y+ther_3.temp_height*8/10 - (ther_3.temp_start_y+ther_3.temp_height/10)-ther_3.delta_start_rect)*i/(maxTemp_plate-minTemp_plate)-ther_3.delta_start_rect);
        }
    }

    //paint actual temperature
    painter.setBrush(QBrush(Qt::black));
    painter.drawEllipse(ther_3.temp_center_x-ther_3.temp_internal_radial,ther_3.temp_center_y-ther_3.temp_internal_radial,ther_3.temp_internal_radial*2,ther_3.temp_internal_radial*2);
    painter.drawRect(ther_3.x1+(ther_3.x2-ther_3.x1)*2/10,ther_3.y1,(ther_3.x2-ther_3.x1)*6/10,-ther_3.delta_start_rect-(ther_3.temp_start_y+ther_3.temp_height*8/10 - (ther_3.temp_start_y+ther_3.temp_height/10) -ther_3.delta_start_rect)*(temp_actual_3-minTemp_plate)/(maxTemp_plate-minTemp_plate));


}

MainWindow::~MainWindow()
{
    delete ui;
}


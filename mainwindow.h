#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <cadran.h>
#include <thermometer.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    cadran volmeter_12V;
    cadran volmeter_5V;
    cadran volmeter_3V;
    cadran fan_speed;
    thermometer ther_1;
    thermometer ther_2;
    thermometer ther_3;

    double volt_3V_actual;
    double volt_5V_actual;
    double volt_12V_actual;
    double speed_actual;
    double temp_actual;
    double temp_actual_2;
    double temp_actual_3;


    double angle_indicator_3V;
    double angle_indicator_5V;
    double angle_indicator_12V;
    double angle_indicator_speed;

    int maxVolt_3V_plate;
    double maxVolt_3V_real;
    int maxVolt_5V_plate;
    double maxVolt_5V_real;
    int maxVolt_12V_plate;
    int maxVolt_12V_real;
    int maxSpeed_plate;
    int maxSpeed_real;

    int maxTemp_plate;
    int minTemp_plate;
    int maxTemp_real;
//    int maxTemp_plate_2;
//    int minTemp_plate_2;
//    int maxTemp_real_2;


protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::MainWindow *ui;
    //timer o


};

#endif // MAINWINDOW_H

#include "mainwindow.h"
#include <QApplication>
#include <QtGui>
#include "getsensors.h"

int main(int argc, char *argv[])
{
    set_reg(1, 0x2c, 0x00, 0x82);	//choose bank2
    set_reg(1, 0x2c, 0x01, 0x00);	//init FCMS
    set_reg(1, 0x2c, 0x10, 0x4d);	//set FAN Output Value
    set_reg(1, 0x2c, 0x00, 0x80);	//choose bank0
    set_reg(1, 0x2c, 0x05, 0xaa);	//set VDSEN14,15,16,17 Voltage
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

#ifndef TIMER_H
#define TIMER_H

#include <QDialog>
#include <timer.h>



class timer:public QWidget
{
   // Q_OBJECT

public:
    timer(QWidget *parent = 0);
    //~timer();

    void draw(QPainter *painter);
};

#endif // TIMER_H

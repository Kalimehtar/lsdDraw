#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <math.h>
#include <QDebug>
#include <QMouseEvent>

#include <time.h>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    void paintEvent(QPaintEvent *);
    float frame = 0;
    QTimer * timer;
    int WIDTH = 600;
    int HEIGHT = 400;
    QImage * image;
    void mousePressEvent(QMouseEvent *e);
    int mouseX;
    int time;
};

#endif // WIDGET_H

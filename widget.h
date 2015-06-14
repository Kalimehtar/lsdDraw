#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <cmath>
#include <QDebug>
#include <QMouseEvent>
#include <QResizeEvent>
#include <vector>
#include <QCheckBox>
#include <QSlider>

#include <time.h>

using namespace std;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e);
    void resizeEvent(QResizeEvent *e);
    QTimer * timer;
    QImage * image;
    float frame = -10;
    int WIDTH = 600;
    int HEIGHT = 400;
    float centerX, centerY;
    int mouseX = 1;
    int time;
    vector< vector< float> > atan;

    QCheckBox *checkbox;
    QCheckBox *makeGlitch;
    QSlider *slider;
    //QSlider *sliderS;

};

#endif // WIDGET_H

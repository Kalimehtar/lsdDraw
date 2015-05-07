#include "widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    image = new QImage(WIDTH, HEIGHT, QImage::Format_RGBA8888);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(16);

    uchar * ptr = (uchar*) image->bits();
    for(int i = 0; i < WIDTH; i++){
        for(int j = 0; j < HEIGHT; j++){
            ptr[j*WIDTH*4 + i*4 + 3] = 255;
        }
    }

}

Widget::~Widget()
{

}

void Widget::mousePressEvent(QMouseEvent *e){
    mouseX = e->pos().x();
}

void Widget::resizeEvent(QResizeEvent *e){
    WIDTH = width();
    HEIGHT = height();

    delete image;
    image = new QImage(WIDTH, HEIGHT, QImage::Format_RGBA8888);
    uchar * ptr = (uchar*) image->bits();
    for(int i = 0; i < WIDTH; i++){
        for(int j = 0; j < HEIGHT; j++){
            ptr[j*WIDTH*4 + i*4 + 3] = 255;
        }
    }
}

void Widget::paintEvent(QPaintEvent *){
    float dt = clock() - time;
    time = clock();
    frame+=mouseX / 20.0f;
    QPainter p(this);
    uchar * ptr = (uchar*) image->bits();
    for(int j = 0; j < HEIGHT; j++){
        //ptr = (uchar*) image->bits();
        //ptr = ptr+WIDTH*4*j;

        for(int i = 0; i < WIDTH; i++){
            float angle = atan2(HEIGHT/2 - j, WIDTH/2 - i);
            float dist =((HEIGHT/2 - j)*(HEIGHT/2 - j) + (WIDTH/2 - i)*(WIDTH/2 - i));
            float power = sin((angle + 10 + frame) * dist * 0.01);
            //float power2 = cos( angle * dist * 0.01 * frame);
            //qDebug() << angle;
            //p.setPen(QColor( (1+sin( (dist + frame * 100)*0.01  )) * 120, (power+ 1) * 120, (1+cos( (dist + frame * 100)*0.01  )) * 120));
            //p.drawPoint(i,j);
//            ptr[i*bytesPerLine + j*4] = (uchar)(1+sin( (dist + frame * 100)*0.01  )) * 120;
//            ptr[i*bytesPerLine + j*4 + 1] = (uchar)(power+ 1) * 120;
//            ptr[i*bytesPerLine + j*4 + 2] = (uchar)(1+cos( (dist + frame * 100)*0.01  )) * 120;
            //line[j*WIDTH + i] = qRgba((1+sin( (sqrt(dist) + frame * 100)*0.01  )) * 120, (power+ 1) * 120, (1+cos( (sqrt(dist) + frame * 100)*0.01  )) * 120, 255);
            ptr[i*4] = (1+sin( (sqrt(dist) + frame * 100)*0.01  )) * 120;
            ptr[i*4 + 1] = (power+ 1) * 128;
            ptr[i*4 + 2] = (1+cos( (sqrt(dist) + frame * 100)*0.01  )) * 120;
            //ptr[j*WIDTH*4 + i*4 + 3] = 255;



        }
        ptr = ptr + WIDTH*4;
    }

    p.drawImage(0,0,*image);
    //p.drawRect(0,0,WIDTH, HEIGHT);
    p.setBrush(QBrush(QColor(255,255,255)));
    p.drawRect(0,0,80,40);
    p.drawText(20,20,QString::number(1000/(dt/1000.0)));
}

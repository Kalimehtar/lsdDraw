#include "widget.h"


float mysin(float x){
    x = x - 6.2831852 * (int)(x * 0.159154946);
    if(x > 3.1415){x = -x + 3.1415926;}
    return x - x*x*x/6 +  x*x*x*x*x/151;
}

float mycos(float x){
    return mysin(x + 3.14159);
}

typedef struct {
    double r;       // percent
    double g;       // percent
    double b;       // percent
} rgb;

    typedef struct {
    double h;       // angle in degrees
    double s;       // percent
    double v;       // percent
} hsv;

    static hsv      rgb2hsv(rgb in);
    static rgb      hsv2rgb(hsv in);

hsv rgb2hsv(rgb in)
{
    hsv         out;
    double      min, max, delta;

    min = in.r < in.g ? in.r : in.g;
    min = min  < in.b ? min  : in.b;

    max = in.r > in.g ? in.r : in.g;
    max = max  > in.b ? max  : in.b;

    out.v = max;                                // v
    delta = max - min;
    if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
        out.s = (delta / max);                  // s
    } else {
        // if max is 0, then r = g = b = 0
            // s = 0, v is undefined
        out.s = 0.0;
        out.h = NAN;                            // its now undefined
        return out;
    }
    if( in.r >= max )                           // > is bogus, just keeps compilor happy
        out.h = ( in.g - in.b ) / delta;        // between yellow & magenta
    else
    if( in.g >= max )
        out.h = 2.0 + ( in.b - in.r ) / delta;  // between cyan & yellow
    else
        out.h = 4.0 + ( in.r - in.g ) / delta;  // between magenta & cyan

    out.h *= 60.0;                              // degrees

    if( out.h < 0.0 )
        out.h += 360.0;

    return out;
}


rgb hsv2rgb(hsv in)
{
    double      hh, p, q, t, ff;
    long        i;
    rgb         out;

    if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
        out.r = in.v;
        out.g = in.v;
        out.b = in.v;
        return out;
    }
    hh = in.h;
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in.v * (1.0 - in.s);
    q = in.v * (1.0 - (in.s * ff));
    t = in.v * (1.0 - (in.s * (1.0 - ff)));

    switch(i) {
    case 0:
        out.r = in.v;
        out.g = t;
        out.b = p;
        break;
    case 1:
        out.r = q;
        out.g = in.v;
        out.b = p;
        break;
    case 2:
        out.r = p;
        out.g = in.v;
        out.b = t;
        break;

    case 3:
        out.r = p;
        out.g = q;
        out.b = in.v;
        break;
    case 4:
        out.r = t;
        out.g = p;
        out.b = in.v;
        break;
    case 5:
    default:
        out.r = in.v;
        out.g = p;
        out.b = q;
        break;
    }
    return out;
}


Widget::Widget(QWidget *parent) : QWidget(parent){
    setAttribute(Qt::WA_NoBackground);
    resize(640,480);
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

    checkbox = new QCheckBox("Make rainbow", this);
    checkbox->setGeometry(5,5,120,15);

    makeGlitch = new QCheckBox("Make glitch", this); // GLITCH эффект напоминающий помехи на ТВ
    makeGlitch->setGeometry(5,25,120,15);

    slider = new QSlider(Qt::Horizontal, this); // Скорость изменения
    slider->setRange(0,100);
    slider->setValue(50);
    slider->setGeometry(5,45,120,15);
}

void Widget::mousePressEvent(QMouseEvent *e){
    centerX = e->pos().x();
    centerY = e->pos().y();
}

void Widget::resizeEvent(QResizeEvent *e){
    WIDTH = width();
    HEIGHT = height();

    delete image;
    image = new QImage(WIDTH, HEIGHT, QImage::Format_RGBA8888);
    image->fill(QColor("red")); // Заполняем изображение любым цветом, потому что изначально альфа канал равен 0

    centerX = WIDTH/2;
    centerY = HEIGHT/2;

    atan.resize(height());
    for(int y = 0; y < height(); y++){
        atan[y].resize(width());
        float distX = (centerY - y)*(centerY - y);
        for(int x = 0; x < width(); x++){
            float dist =(distX + (centerX - x)*(centerX - x)) * 0.04;
            atan[y][x] = atan2(centerY - y, centerX - x)  + mysin(dist*dist*0.0000005);
        }
    }
}

void Widget::paintEvent(QPaintEvent *){
    float dt = clock() - time;
    time = clock();

    frame += pow(2.7, (slider->value() - 50)/15.0f )/500.0f;

    float cur_frame = frame + 0.5*mysin(frame);
    int GlitchPower = floor(2*(1+sin(frame*10)));
    qDebug() << GlitchPower;

    uchar * ptr = (uchar*) image->bits();
    for(int j = 0; j < HEIGHT-3; j++){
        float distX = (centerY - j)*(centerY - j);
        for(int i = 0; i < WIDTH; i++){

            float dist =((distX + (centerX - i)*(centerX - i)) * 0.04);
            float angle = fmod (atan[j][i] + frame  , 3*abs(mysin(frame))+0.05);
            float power = mysin(angle * dist * 0.005);
            float tmp = sqrt(dist) * 0.223 + cur_frame + angle*4;

            if(checkbox->isChecked()){
                rgb color;
                color.r = (1+mysin(tmp)) * 127;
                color.g = (power+ 1) * 127;
                color.b = (1+(mycos(tmp*2))) * 127;

                hsv colorH = rgb2hsv(color);
                colorH.h = fmod(colorH.h+frame , 360);
                colorH.v = 255;
                colorH.s = colorH.s*0.5 + 0.5;
                color = hsv2rgb(colorH);

                ptr[i*4     ] = color.r;
                ptr[i*4 + 1 ] = color.g;
                ptr[i*4 + 2 ] = color.b;
            }else{
                ptr[i*4     ] = (1+mysin(tmp)) * 127;
                ptr[i*4 + 1 ] = (power+ 1) * 127;
                ptr[i*4 + 2 ] = (1+(mycos(tmp*2))) * 127;
            }
        }

        if(makeGlitch->isChecked()){
            ptr = ptr + WIDTH*4 + (rand() % (GlitchPower*2+1) - GlitchPower); // ГЛИТЧ ЭФФЕКТ
        }else{
            ptr = ptr + WIDTH*4;
        }
    }

    QPainter p(this);
    p.drawImage(0,0,*image);

    p.setBrush(QBrush(QColor(50,50,50)));
    p.setPen(QPen(QColor(35,35,35)));
    p.drawRect(0,0,130,80);

    p.setPen(QPen(QColor(255,255,255)));
    p.drawText(5,75,"FPS : " + QString::number(1000/(dt/1000.0)));

}

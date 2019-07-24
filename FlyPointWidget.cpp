#include "FlyPointWidget.h"

#include <QPainter>
#include <QTimer>


FlyPointWidget::FlyPointWidget(QWidget *parent) :
    QWidget(parent)
{
    timer = new QTimer(this);
    QObject::connect(timer, &QTimer::timeout, [=](){
        //делаем яркость тише
        aplha = aplha - 10;
        if (aplha <= 30) {
            aplha = 30;
            timer->stop();
        } else {
            colorP.setAlpha(aplha);
            update();
        }

        //setWindowOpacity(255 / aplha)
        //this->move(1, 1);
        //

    });
    //    timer->start(100);
    qrand();
    path.setX((rand() % 3) - 1);
    path.setY((rand() % 3) - 1);


    setToolTipDuration(0);
}

FlyPointWidget::~FlyPointWidget()
{
    timer->stop();
}

void FlyPointWidget::setLight()
{
    aplha = 255;
    update();
    timer->start(100);


}

bool FlyPointWidget::isNotLight() const
{
    return aplha == 30;
}

void FlyPointWidget::moveToPath()
{
    this->move(this->pos().x() + path.x(),
               this->pos().y() + path.y());
}

void FlyPointWidget::paintEvent(QPaintEvent *)
{
//    QRadialGradient grad(width() / 2, height() / 2, height() / 2, width() / 2, height() / 2);

//    grad.setColorAt(1, colorP);

    QPainter painter(this);
    //рисуем точку и яркость
    QPen pen(colorP);
    pen.setWidth(1);
    painter.setPen(Qt::NoPen);

    painter.setBrush(QBrush(colorP));
    painter.drawEllipse(this->rect());


    QRadialGradient grad(width() / 2, height() / 2, height() / 2);
    grad.setFocalPoint(width() / 2, height() / 2);

    grad.setColorAt(0, colorP);
    grad.setColorAt(1, colorB);
    painter.setBrush(grad);
    painter.drawEllipse(this->rect());

    setToolTip(
                QString("Точка\r\nКоординаты:\r\nx:%1\r\ny:%2")
                .arg(this->pos().x())
                .arg(this->pos().y()));
}

QPoint FlyPointWidget::getPath() const
{
    return path;
}

void FlyPointWidget::mouseDoubleClickEvent(QMouseEvent *)
{
    path.setX((rand() % 3) - 1);
    path.setY((rand() % 3) - 1);
}


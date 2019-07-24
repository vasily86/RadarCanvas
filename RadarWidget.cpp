#include "RadarWidget.h"

#include <QPainter>
#include <QTimer>
#include <QDebug>
#include <random>
#include <QDateTime>
#include <math.h>

RadarWidget::RadarWidget(QWidget *parent) :
    QWidget(parent)
{
    fontBigText = new QFont("GOST Type BU", 10);
    fontBigText->setBold(true);

    connect(this, &RadarWidget::changeAngle, [=](int angle){


        int x_center = width() / 2;
        int y_center = height() / 2;
        qreal x = radius * cos(angle * M_PI/ 180.0);
        qreal y = radius * sin(angle * M_PI/ 180.0);

        //Расчёты для линии радара для одного угла одинаковы для всех точке
        //просчитываем один раз
        QPoint centerP = QPoint(x_center, y_center);
        QPoint p2 = QPoint(x + x_center , y + y_center);

        //расчёт высоты трапеции до центра радиуса
        qreal down = pow(p2.x() - centerP.x(), 2) +
                     pow(p2.y() - centerP.y(), 2);

        //проверка коллизий
        QMutableListIterator<FlyPointWidget *> it(listWidgets);
        while (it.hasNext()) {
            FlyPointWidget * w = it.next();

            QPoint p0 = w->pos();


            //расчёт высоты трапеции до центра радиуса
            qreal upper = ((p2.x() - centerP.x()) * (p0.y() - centerP.y())) -
                          ((p2.y() - centerP.y()) * (p0.x() - centerP.x()));


           qreal h = abs(upper / sqrt(down));
           if ( h < 8 ){
               QPointF av;
               av.setX( centerP.x() - p0.x() );
               av.setY( centerP.y() - p0.y() );

               QPointF bv;
               bv.setX( p2.x() - centerP.x() );
               bv.setY( p2.y() - centerP.y() );

               qreal sa = sqrt(pow(av.x(),2) + pow(av.y(),2));
               qreal sb = sqrt(pow(bv.x(),2) + pow(bv.y(),2));

               qreal cosAB = (av.x()*bv.x() + av.y()*bv.y()) / (sa*sb) ;
//              qreal rad = acosf( cosAB );
//               qreal grad = (rad * 180)/ M_PI;
//               qDebug() << "sa " << sa << " sb "
//                        << sb << "cosAB " << cosAB
//                        << " po " << p0
//                        << " grad " << grad;
//               << " grad " << grad;
//                                   << " ugol " << ;
               if ( cosAB < 0 && (sa <= radius)) {
                   w->setLight();
                   continue;
               }
           }

           //если длинна вектора от точки до центра больше длинны радиуса - то точка ушла за край
           QPointF av;
           av.setX( centerP.x() - p0.x() );
           av.setY( centerP.y() - p0.y() );

           qreal sa = sqrt(pow(av.x(),2) + pow(av.y(),2));

           if (w->isNotLight() && sa > radius) {
               it.remove();
               w->deleteLater();
           } else {
                w->moveToPath();
           }

        }
    });





}

void RadarWidget::startRotate()
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [=](){
        angle += 2;
        update();
        emit changeAngle(angle);
    });
    timer->start(20);
}

void RadarWidget::stopRotate()
{
    timer->stop();
}

//!< Раскидать точки по плоскости */
void RadarWidget::addPoints(int count)
{
    //раскидаем точки по плоскости
    std::uniform_int_distribution<> dist{0, width()};
    std::default_random_engine engn( QDateTime::currentDateTime().toTime_t() );

    for (int i = 0; i < count; ++i) {
        FlyPointWidget * widget = new FlyPointWidget(this);
        widget->setGeometry(dist(engn), dist(engn), 20, 20);
        widget->show();
        //widget->setLight();

        listWidgets.append(widget);
    }
}

/*! Удалить все объекты точки радара */
void RadarWidget::clearPoints()
{
    QMutableListIterator<FlyPointWidget *> it(listWidgets);
    while (it.hasNext()) {
        FlyPointWidget * w = it.next();
        it.remove();
        w->deleteLater();
    }
}

void RadarWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.save();
    //отрисовываем задний фон
    painter.drawImage(this->rect(), *backImg.get());
    painter.restore();


    //Отрисовка элемента линии и pie которые и представляют "радар"
    painter.setBrush( QColor(23, 121, 53) );
    painter.setOpacity(0.5);
    int x_center = width() / 2;
    int y_center = height() / 2;
    qreal x = radius * cos(angle * M_PI/ 180.0);
    qreal y = radius * sin(angle * M_PI/ 180.0);

    painter.drawPie(x_center - radius, y_center - radius, radius*2, radius*2, -angle*16, 10*16);
    QPen pen( QColor(Qt::green));
    pen.setWidth(3);
    painter.setPen(pen);
    painter.drawLine(x_center, y_center, x + x_center , y + y_center);


    //отрисовка текста в нижнем углу
    painter.setFont(*fontBigText);
    painter.drawText(10, height() - 40, 300, 20,
                     Qt::AlignLeft, QString("Текущий угол %1\u00b0").arg(360 - (angle % 360)));
    painter.drawText(10, height() - 20, 300, 20,
                     Qt::AlignLeft, QString("Кол-во объектов %1").arg(listWidgets.count()));

}

/*! Формирование сетки заднего фона */
void RadarWidget::prepareGrid()
{
    backImg = std::make_shared<QImage>(this->width(), this->height(), QImage::Format_RGB32);
    QPainter painter;
    //painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.begin(backImg.get());
    //painter.setRenderHint(QPainter::Antialiasing);
    {
        painter.save();
        QPen penBack(Qt::black);
        painter.setPen(penBack);
        QBrush brash(Qt::black);
        painter.setBrush(brash);

        painter.drawRect(0, 0, this->width(), this->height());
        painter.restore();
    }

    QPen pen( QColor(15, 77, 33));
    pen.setWidth(1);
    painter.setPen(pen);
    for (int i = 0; i < this->width(); i=i+10) {
        painter.drawLine(i, 0, i, this->height());
    }
    for (int i = 0; i < this->height(); i=i+10) {
        painter.drawLine(0, i, this->width(), i);
    }

    painter.end();
}

/*! Формирование оси x и y, кругов, подписей для заднего фона */
void RadarWidget::prepareAxes()
{
    QPainter painter;
    //painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.begin(backImg.get());
    painter.setRenderHint(QPainter::Antialiasing);

    int width = this->width();
    int height = this->height();

    QPen pen( QColor(23, 121, 53));
    painter.setPen(pen);


    //рисуем окружности
    {
        pen.setWidth(1);
        painter.setPen(pen);
        painter.save();
        for (int i = 0; i < height / 2 - 20; i = i + 30) {

            //каждое третье кольцо жирнее
            if (i % 90 == 0) {
                pen.setWidth(2);
                pen.setColor(Qt::green);
            } else {
                pen.setWidth(1);
                pen.setColor(QColor(23, 121, 53));
            }
            painter.setPen(pen);

            painter.drawEllipse(QPoint(width / 2, height / 2), i, i);            

            radius = i;
        }
        painter.restore();
    }

    //Каждые 10 градусов раскидываем линии
    int x_center = this->width() / 2;
    int y_center = this->height() / 2;
    for (int i = -5 ; i < 355; i = i + 10) {
        qreal x = (radius + 15) * cos(i * M_PI/ 180.0);
        qreal y = (radius + 15) * sin(i * M_PI/ 180.0);

        painter.drawLine(x_center, y_center, x + x_center , y + y_center);
    }

    pen.setColor(colorP);
    pen.setWidth(2);
    painter.setPen(pen);

    painter.setFont(*fontBigText);
    //Каждые 10 градусов раскидываем текст
    for (int i = 0 ; i < 360; i = i + 10) {
        qreal x = (radius + 20) * cos(i * M_PI/ 180.0);
        qreal y = (radius + 20) * sin(i * M_PI/ 180.0);

        painter.drawText(x + x_center - 100, y + y_center - 15, 200, 30,
                         Qt::AlignCenter, QString("%1\u00b0").arg(i == 0 ? 0 : 360 - i));
    }

    //Рисуем оси x и y
    pen.setWidth(2);

    painter.drawLine(width / 2 , y_center - radius, width / 2, y_center + radius);
    painter.drawLine(x_center - radius, height / 2, x_center + radius, height / 2);

    painter.end();
}

void RadarWidget::resizeEvent(QResizeEvent *)
{
    prepareGrid();
    prepareAxes();

    //Thing about
    if (radius < 300)
        fontBigText->setWeight(8);
    else
        fontBigText->setWeight(10);

    fontBigText->setBold(true);
}

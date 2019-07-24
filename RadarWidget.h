#ifndef RADARWIDGET_H
#define RADARWIDGET_H

#include <QWidget>
#include <memory>
#include <qbrush.h>
#include "FlyPointWidget.h"

class RadarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RadarWidget(QWidget* parent = 0);


    void startRotate();
    void stopRotate();

    void addPoints(int count);
    void clearPoints();

protected:
    void paintEvent(QPaintEvent *);

private:
    void prepareGrid();
    void prepareAxes();

    std::shared_ptr<QImage> backImg;        //!< Картинка для заднего фона
    QFont * fontBigText;                    //!< Шрифт для надписей
    QColor colorP{17, 227, 54, 200};        //!< Зелёный цвет, с небольшой прозрачностью


    QList<FlyPointWidget *> listWidgets;    //!< Список, где лежат наши точки

    int angle{0};                           //!< Угол поворота
    int radius;                             //!< Радиус последнего круга
    QTimer * timer;                         //!< Таймер, генерирующий поворот "стрелы" радара
protected:
    void resizeEvent(QResizeEvent *);

signals:
    void changeAngle(int angle);            //!< Сигнал, испускающийся при изменении угла "стрелы" радара
};

#endif // RADARWIDGET_H

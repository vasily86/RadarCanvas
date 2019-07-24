#ifndef FLYPOINTWIDGET_H
#define FLYPOINTWIDGET_H

#include <QWidget>

class FlyPointWidget : public QWidget
{
    Q_OBJECT
public:
    FlyPointWidget(QWidget* parent = 0);
    ~FlyPointWidget();
    void setLight();

    bool isNotLight() const;

    void moveToPath();

protected:
    void paintEvent(QPaintEvent *);

private:
    QColor colorP{17, 227, 54, 30};
    QColor colorB{0, 0, 0, 200};
    int aplha{30};
    QTimer * timer;
    QPoint path;    //Траектория полёта

    QPoint getPath() const;

    // QWidget interface
protected:
    void mouseDoubleClickEvent(QMouseEvent *);
};

#endif // FLYPOINTWIDGET_H

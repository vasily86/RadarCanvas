#include "mainwindow.h"
#include <QApplication>

#include "RadarWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setStyleSheet("QToolTip {color: #ffffff; background-color: #11e336;border: 1px solid white; }");

    MainWindow w;
    w.show();

//    RadarWidget radar;
//    radar.setGeometry(1950, 20, 1200, 1000);
//    radar.show();

    return a.exec();
}

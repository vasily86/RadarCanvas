#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->widget->addPoints(ui->spinBox->value());
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->widget->startRotate();
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->widget->stopRotate();
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->widget->clearPoints();
}

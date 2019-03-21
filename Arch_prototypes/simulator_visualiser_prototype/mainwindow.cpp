#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // SOURCE: https://www.iconfinder.com/iconsets/buttons-9
    // SOURCE: https://www.roughguides.com/maps/europe/belgium/
    QPixmap pmap_mapOfBelgium(":/img/map_of_belgium.png");
    int width = ui->label_map->width();
    int height = ui->label_map->height();
    ui->label_map->setPixmap(pmap_mapOfBelgium.scaled(width, height, Qt::KeepAspectRatio));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpenSimulationFile_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open simulation file"), "/home", tr("CSV files (*.csv);;All files (*)"));

    // process file

    //QPainter::drawEllipse(QPointF(100,100), 50,50);


}

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QPainter>
#include <QPaintEvent>

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // SOURCE: https://www.iconfinder.com/iconsets/buttons-9
    // SOURCE: https://www.roughguides.com/maps/europe/belgium/
//    QPixmap pmap_mapOfBelgium(":/img/map_of_belgium.png");
//    int width = ui->label_map->width();
//    int height = ui->label_map->height();
//    ui->label_map->setPixmap(pmap_mapOfBelgium.scaled(width, height, Qt::KeepAspectRatio));

    this->setFixedSize(660, 600);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpenSimulationFile_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open simulation file"), "/home", tr("CSV files (*.csv);;All files (*)"));

    // process data into internal representation (using vector, map, etc)

    if(filename != nullptr)
    {
        m_drawMapData = true;


        this->update();
    }
}

void M_drawCircle(QPainter& painter, QPointF pos, QColor color, int radius)
{
    painter.setBrush(color);
    painter.drawEllipse(pos, radius, radius);
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QPainter painter{this};

    QSize mapSize{this->ui->centralWidget->width()-20, this->ui->centralWidget->height()-20};
    QPixmap pmap_mapOfBelgium = QPixmap(":/img/map_of_belgium.png").scaled(mapSize, Qt::KeepAspectRatio);

    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.drawPixmap(10, 47, pmap_mapOfBelgium);

    if(m_drawMapData || true)
    {
        // brussels
        M_drawCircle(painter, QPointF{300,250}, QColor{255,0,0,255}, 50);

        // antwerp
        M_drawCircle(painter, QPointF{300,150}, QColor{255,0,0,255}, 32);

        // hasselt
        M_drawCircle(painter, QPointF{449,214}, QColor{255,0,0,255}, 20);

        // mechelen
        M_drawCircle(painter, QPointF{313,188}, QColor{255,0,0,255}, 20);

        // gent
        M_drawCircle(painter, QPointF{198,187}, QColor{255,0,0,255}, 28);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    std::cout << e->x() << ", " << e->y() << std::endl;
}



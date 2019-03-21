#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QPainter>
#include <QPaintEvent>
#include <QMessageBox>

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // SOURCE: https://www.iconfinder.com/iconsets/buttons-9
    // SOURCE: https://www.iconfinder.com/iconsets/common-toolbar
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

// TODO position needs to be relative to the image.
void M_drawCircle(QPainter& painter, QPointF pos, QColor color, int radius)
{
    painter.setPen(color);
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

    if(m_fileOpened || true)
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

void MainWindow::on_actionOpenSimulationFile_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open simulation file"), "/home", tr("CSV files (*.csv);;All files (*)"));

    // process data into internal representation (using vector, map, etc)

    if(filename != nullptr)
    {
        std::cout << "PROCESSING '" << filename.toStdString() << "'." << std::endl;

        m_fileOpened = true;
        this->update();
    }
    else
    {
        std::cout << "NO FILE SELECTED, IGNORE" << std::endl;
    }
}

void MainWindow::on_actionSaveToImage_triggered()
{
    if(!m_fileOpened)
    {
        std::cout << "ERROR, CANNOT SAVE FILE SINCE NO DATA IS PRESENT." << std::endl;

        QMessageBox msgBox;
        msgBox.setText("Please open simulation data before saving to image.");

        msgBox.exec();

        return;
    }

    QString filename = QFileDialog::getSaveFileName(this, tr("Save simulation images"), "/home", tr("GIF files (*.csv);;ZIP files (*.zip)"));

    if(filename != nullptr)
    {
        std::cout << "SAVING TO '" << filename.toStdString() << "'." << std::endl;

    }
    else
    {
        std::cout << "NO FILE SELECTED, IGNORE" << std::endl;
    }
}

void MainWindow::on_actionControlsBackFull_triggered()
{
    std::cout << "TO BEGIN" << std::endl;
}

void MainWindow::on_actionControlsAutoPlayPause_triggered()
{
    std::cout << "PLAY / PAUSE" << std::endl;
}

void MainWindow::on_actionControlsForwardFull_triggered()
{
    std::cout << "TO END" << std::endl;
}

void MainWindow::on_actionControlsBackOne_triggered()
{
    std::cout << "PREVIOUS" << std::endl;
}

void MainWindow::on_actionControlsForwardOne_triggered()
{
    std::cout << "NEXT" << std::endl;
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    //std::cout << e->x() << ", " << e->y() << std::endl;
}

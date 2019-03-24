#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpenSimulationFile_triggered();

    void on_actionControlsBackFull_triggered();

    void on_actionControlsAutoPlayPause_triggered();

    void on_actionControlsForwardFull_triggered();

    void on_actionControlsBackOne_triggered();

    void on_actionControlsForwardOne_triggered();

    void on_actionSaveToImage_triggered();

private:
    Ui::MainWindow *ui;

private:
    bool m_fileOpened = false;

protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QObject>
#include <QMainWindow>

#include "camera.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int cropLeft = 0;
    int cropRight = 0;
	QImage MainWindow::Crop(QImage);
	void MainWindow::Setup();


private slots:
    void on_RecStopBtn_clicked();
    void timerslot();
    void DisplayLeft();
    void DisplayRight();

    void on_LeftCamCropSlider_valueChanged(int value);
    void on_RightCamCropSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    QTimer* myTimer;
	QImage m_final_image_left;
	QImage m_final_image_right;

    Camera *m_cam;

    };




#endif // MAINWINDOW_H

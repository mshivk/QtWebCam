#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QTimer>
#include<QImage>
#include<QTime>
#include<qDebug>

#include "camera.h"


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);

    Setup();
}

void MainWindow::Setup(){
    //cropLeft = 0;
    //cropRight=0;
    myTimer = new QTimer(this);
    myTimer->setInterval(0);
    myTimer->start();
    myTimer->setSingleShot(false);

    int w = ui->LeftCamImg->width();
    int h = ui->LeftCamImg->height();
    m_cam = new Camera(w,h);
    m_cam->initialize();

	connect(myTimer, SIGNAL(timeout()), this, SLOT(DisplayLeft()));
	connect(myTimer, SIGNAL(timeout()), this, SLOT(DisplayRight()));

}

MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::on_RecStopBtn_clicked()
{
	if (ui->RecStopBtn->text() == "Record") {
		ui->RecStopBtn->setText("Stop");

		if (myTimer->isActive() == true) {
			qDebug() << "Timer is active" << endl;
			disconnect(myTimer, SIGNAL(timeout()), this, SLOT(DisplayLeft()));
			disconnect(myTimer, SIGNAL(timeout()), this, SLOT(DisplayRight()));
			myTimer->stop();
		}
		// Disable the sliders 
		Setup();
     } else if(ui->RecStopBtn->text() == "Stop"){
        if (myTimer->isActive() == true){
			disconnect(myTimer, SIGNAL(timeout()), this, SLOT(DisplayLeft()));
			disconnect(myTimer, SIGNAL(timeout()), this, SLOT(DisplayRight()));
			myTimer->stop();
		}
        ui->RecStopBtn->setText("Record");
		// Enable the sliders 

        //QPixmap pix2("");
        //ui->LeftCamImg->setPixmap(pix2);

    }

}

void MainWindow :: DisplayLeft() {

    m_cam->capture(cropLeft, cropRight);
    QImage img(m_cam->getImage());
    qDebug() << "Size after cropping : " << img.size();
    ui->LeftCamImg->setPixmap(QPixmap::fromImage(img));
    ui->LeftCamImg->show();
    repaint();

    /*
	Ptr<Data> data = m_devicemanager.Pop( 0 );
	if ( data.ptr() )
	{
		cv::Mat mat;
		cv::cvtColor( data->m_mat, mat, CV_BayerRG2BGR );
		if ( 1 ) // resize
		{
			cv::Mat m2;
			cv::Size target(ui->LeftCamImg->width(),ui->LeftCamImg->height());
			cv::resize(mat,m2, target );
			mat = m2;
		}
		
		QImage img(mat.data, mat.cols, mat.rows, mat.step.p[0],QImage::Format_RGB888,Q_NULLPTR, Q_NULLPTR );
		m_final_image_left = Crop(img);
		ui->LeftCamImg->setPixmap( QPixmap::fromImage(m_final_image_left));
		ui->LeftCamImg->show();
	}
    */
	
}
QImage MainWindow::Crop(QImage full_image) {

	int height_ = ui->LeftCamImg->height();
	int width_ = ui->LeftCamImg->width();

	float BarTop = height_ * (cropLeft*.01);
	float BarBottom = height_* (cropRight*.01);

	QImage cropped_image = full_image.copy(0, BarTop, width_, height_); // top is cropped and image is pushed up and centered.
	if (cropRight != 0) {
		QImage imag2 = cropped_image.copy(0, 0, width_, height_ - BarBottom - BarTop);
		cropped_image = imag2.copy();
	}

	return cropped_image;

}
void MainWindow :: DisplayRight(){

    m_cam->capture(cropLeft,cropRight);
    QImage img(m_cam->getImage());
    ui->RightCamImg->setPixmap(QPixmap::fromImage(img));
    ui->RightCamImg->show();
    repaint();

}

void MainWindow :: timerslot()
{
    qDebug() << "timeslot";
}

void MainWindow::on_LeftCamCropSlider_valueChanged(int value)
{
    qDebug() << "Left Cam crop :" << value;
    cropLeft = value;
}

void MainWindow::on_RightCamCropSlider_valueChanged(int value)
{
    //qDebug() << "Right Cam crop :" << value;
    cropRight = value;


}

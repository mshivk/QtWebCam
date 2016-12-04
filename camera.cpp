#include "camera.h"


Camera :: Camera(int width, int height, QObject *parent) : QObject(parent),width_(width),height_(height) {
    capture_.mWidth = width;
    capture_.mHeight = height;
    capture_.mTargetBuf = new int[width * height];
    int devices = setupESCAPI();
    qDebug() << devices << endl;
    for (int i = 0; i < devices; i++)
    {
        char temp[256];
        getCaptureDeviceName(i, temp, 256);
        qDebug() << "Device number"<< i <<"name"<< temp;

    }
    if (devices == 0)
        {
            qDebug() << "[Camera] ESCAPI initialization failure or no devices found";
        }
}

Camera :: ~Camera(){
    deinitCapture(0);
}

int Camera :: initialize(){


    if (initCapture(0,&capture_)==0){
        qDebug() << "[Camera] Capture failed - device may already be in use";
        return -2;
    }
    //qDebug() << "Camera is initialized at:" << QTime::currentTime().toString();
    return 0;
}


int Camera :: capture(int crop_top, int crop_bottom){

    float BarTop = height_ * (crop_top*.01);
    float BarBottom =  height_* (crop_bottom*.01);
    doCapture(0);
    while (isCaptureDone(0) == 0);

    image_ = QImage(width_,height_,QImage::Format_RGB32);

    for(int y(0); y < height_ ; ++y) {
        for (int x(0); x < width_ ; ++x ){
            int index(y * width_ + x);
            image_.setPixel(x,y,capture_.mTargetBuf[index]);
        }
    }


    QImage imag = image_.copy(0,BarTop,width_,height_); // top is cropped and image is pushed up and centered.
    //qDebug() << "Imag size is: " << imag.size();
    image_ = imag.copy();
    if (crop_bottom != 0){
        QImage imag2 = image_.copy(0,0,width_,height_-BarBottom-BarTop);
        image_ = imag2.copy();
    }

    return 1;
}

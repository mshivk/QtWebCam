#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QImage>
#include<qDebug>
#include "escapi.h"



class Camera : public QObject
{
Q_OBJECT

public:
    explicit Camera(int width, int height, QObject *parent = 0);
    ~Camera();
    int initialize();
    void deinitialize();
    int capture( int cropL, int cropR);
    const QImage& getImage() const { return image_; }
    //const int* getImageRaw() const { return capture_.mTargetBuf; }

private:
    int width_;
    int height_;
    QImage image_;

    struct SimpleCapParams capture_;


};


#endif // CAMERA_H

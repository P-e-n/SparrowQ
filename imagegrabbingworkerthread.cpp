#include "imagegrabbingworkerthread.h"
#include <Qmutex>
ImageGrabbingWorkerThread::ImageGrabbingWorkerThread(Dothinkey* dk, QObject *parent)
    :forceStop(false)
{
    this->dk = dk;
}

void ImageGrabbingWorkerThread::run()
{
    forceStop = false;
    while(true) {
        if(this->forceStop) break;
        QImage* newImage =  dk->DothinkeyGrabImage(0);
        emit imageChanged(*newImage);
        delete newImage;
        QThread::msleep(100); //Slow down the cpu cooldown
    }
}

void ImageGrabbingWorkerThread::stop()
{
    forceStop = true;
}


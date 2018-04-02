#include "imagegrabbingworkerthread.h"

ImageGrabbingWorkerThread::ImageGrabbingWorkerThread(Dothinkey* dk, QObject *parent)
    :forceStop(false)
{
    this->dk = dk;
}

void ImageGrabbingWorkerThread::run()
{
    while(true) {
        //QMutex mutex;
        // prevent other threads from changing the "Stop" value
        //mutex.lock();
        if(this->forceStop) break;
        //mutex.unlock();
        QImage* image = new QImage();
        dk->DothinkeyGrabImage(0, *image);
        emit imageChanged(*image);
        QThread::msleep(500); //Slow down the cpu cooldown
    }
}

void ImageGrabbingWorkerThread::stop()
{
    forceStop = true;
}


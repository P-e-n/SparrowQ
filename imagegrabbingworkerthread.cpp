#include "imagegrabbingworkerthread.h"

imageGrabbingWorkerThread::imageGrabbingWorkerThread(QObject *parent)
    :forceStop(false)
{

}

void imageGrabbingWorkerThread::run()
{
    for (int i = 0; i < 10; i++) {
        if (forceStop) return;
        emit done();
    }
}

void imageGrabbingWorkerThread::stop()
{
    forceStop = true;
}

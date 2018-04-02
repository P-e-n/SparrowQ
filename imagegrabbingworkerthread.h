#ifndef IMAGEGRABBINGWORKERTHREAD_H
#define IMAGEGRABBINGWORKERTHREAD_H
#include <QThread>
#include <QImage>
#include "dothinkey.h"

class ImageGrabbingWorkerThread : public QThread
{
    Q_OBJECT
public:
    ImageGrabbingWorkerThread(Dothinkey* dk, QObject *parent = 0);
    void stop();

protected:
    void run();
    bool forceStop;

private:
    Dothinkey* dk;

signals:
    void done();
    void imageChanged(QImage);
};

#endif // IMAGEGRABBINGWORKERTHREAD_H

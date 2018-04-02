#ifndef IMAGEGRABBINGWORKERTHREAD_H
#define IMAGEGRABBINGWORKERTHREAD_H
#include <QThread>

class imageGrabbingWorkerThread : public QThread
{
    Q_OBJECT
public:
    imageGrabbingWorkerThread(QObject *parent = 0);
    void stop();
protected:
    void run();
    bool forceStop;
signals:
    void done();
};

#endif // IMAGEGRABBINGWORKERTHREAD_H

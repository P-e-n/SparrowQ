#ifndef SPARROW_H
#define SPARROW_H

#include <QMainWindow>
#include "dothinkey.h"
#include "imagegrabbingworkerthread.h"
#include <QGraphicsScene>

namespace Ui {
class Sparrow;
}

class Sparrow : public QMainWindow
{
    Q_OBJECT

public:
    explicit Sparrow(QWidget *parent = 0);
    ~Sparrow();
    ImageGrabbingWorkerThread *imageThread;

public slots:
    void on_image_changed(QImage image);

private slots:
    void on_openCameraButton_clicked();
    void on_closeCameraButton_clicked();

private:
    Ui::Sparrow *ui;
    QGraphicsScene * scene = new QGraphicsScene();
    Dothinkey *imageGrabber;
};

#endif // SPARROW_H

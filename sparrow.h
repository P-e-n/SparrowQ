#ifndef SPARROW_H
#define SPARROW_H

#include <QMainWindow>
#include "dothinkey.h"
#include "logger.h"

namespace Ui {
class Sparrow;
}

class Sparrow : public QMainWindow
{
    Q_OBJECT

public:
    explicit Sparrow(QWidget *parent = 0);
    ~Sparrow();

private slots:
    void on_enumerateDeviceButton_clicked();
    void on_openCameraButton_clicked();
    void on_testOpenBMP_clicked();

private:
    Ui::Sparrow *ui;
    Dothinkey *imageGrabber;
    Logger* logger;
};

#endif // SPARROW_H

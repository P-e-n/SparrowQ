#include "sparrow.h"
#include "ui_sparrow.h"
#include "logger.h"
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

Sparrow::Sparrow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Sparrow)
{
    setWindowIcon(QIcon(":/images/sparrow.png"));
    ui->setupUi(this);

    log_editor = ui->loggerPlainTextEdit;

    qInfo("SparrowQ started");

    QPixmap qp = QPixmap(":/images/sparrow.png");
    if (qp.isNull()) {
        qCritical("Cannot open bmp file!");
    } else {
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(qp);
        scene->addItem(item);
        ui->testBmpView->setScene(scene);
        ui->testBmpView->show();
    }
    imageGrabber = new Dothinkey(nullptr);
    imageThread = new ImageGrabbingWorkerThread(imageGrabber, this);
    connect(imageThread, SIGNAL(imageChanged(QImage)), this, SLOT(on_image_changed(QImage)));
}

Sparrow::~Sparrow()
{
    // Any log written after this line will not be shown in the UI
    //  (Remark: not sure if this will cause raise-condition in log handler)
    log_editor = Q_NULLPTR;

    delete ui;
}

void Sparrow::on_openCameraButton_clicked()
{
    qInfo("OpenDevice clicked");
    const int channel = 0;
    bool res = imageGrabber->DothinkeyEnum();
    if (!res) { qCritical("Cannot find dothinkey"); return; }
    imageGrabber->DothinkeyOpen();
    if (!res) { qCritical("Cannot open dothinkey"); return; }
    imageGrabber->DothinkeyLoadIniFile(channel);
    if (!res) { qCritical("Cannot load dothinkey ini file"); return; }
    imageGrabber->DothinkeyStartCamera(channel);
    if (!res) { qCritical("Cannot start camera"); return;}
    ui->openCameraButton->setEnabled(false);
    ui->closeCameraButton->setEnabled(true);
    //imageGrabber->DothinkeyGrabbingThread(true);
    //imageThread->start();
}

void Sparrow::on_closeCameraButton_clicked()
{
    ui->openCameraButton->setEnabled(true);
    ui->closeCameraButton->setEnabled(false);
    imageGrabber->DothinkeyClose();
}

//Received Signals....

void Sparrow::on_image_changed(QImage image)
{
    QPixmap pixmap = QPixmap::fromImage(image);
    QPixmap qp = QPixmap(":/images/Flamingo.bmp");
    scene->clear();
    scene->addPixmap(qp);
    ui->testBmpView->setScene(scene);
    ui->testBmpView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

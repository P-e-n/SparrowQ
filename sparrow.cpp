#include "sparrow.h"
#include "ui_sparrow.h"
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

Sparrow::Sparrow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Sparrow)
{
    setWindowIcon(QIcon(":/images/sparrow.png"));
    ui->setupUi(this);
    QString fileName = "logger.txt";
    logger = new Logger(this, fileName, ui->loggerPlainTextEdit);
    logger->write("SparrowQ Started!");
    QPixmap qp = QPixmap(":/images/sparrow.png");
    if (qp.isNull()) {
        logger->write("Cannot open bmp file!");
    } else {
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(qp);
        scene->addItem(item);
        ui->testBmpView->setScene(scene);
        ui->testBmpView->show();
    }
    imageGrabber = new Dothinkey(logger, nullptr);
    imageThread = new ImageGrabbingWorkerThread(imageGrabber, this);
    connect(imageThread, SIGNAL(imageChanged(QImage)), this, SLOT(on_image_changed(QImage)));
}

Sparrow::~Sparrow()
{
    delete ui;
}

void Sparrow::on_openCameraButton_clicked()
{
    logger->write("OpenDevice clicked");
    const int channel = 0;
    bool res = imageGrabber->DothinkeyEnum();
    if (!res) { logger->write("Cannot find dothinkey"); return; }
    imageGrabber->DothinkeyOpen();
    if (!res) { logger->write("Cannot open dothinkey"); return; }
    imageGrabber->DothinkeyLoadIniFile(channel);
    if (!res) { logger->write("Cannot load dothinkey ini file"); return; }
    imageGrabber->DothinkeyStartCamera(channel);
    if (!res) { logger->write("Cannot start camera"); return;}
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

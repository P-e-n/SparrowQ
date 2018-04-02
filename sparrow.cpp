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
    imageGrabber = new Dothinkey(logger, nullptr);
    imageThread = new ImageGrabbingWorkerThread(imageGrabber, this);
    connect(imageThread, SIGNAL(imageChanged(QImage)), this, SLOT(on_image_changed(QImage)));
}

Sparrow::~Sparrow()
{
    delete ui;
}

void Sparrow::on_enumerateDeviceButton_clicked()
{
    const int channel = 0;
    bool res = imageGrabber->DothinkeyEnum();
    if (!res) { logger->write("Cannot find dothinkey"); return; }
    imageGrabber->DothinkeyOpen();
    if (!res) { logger->write("Cannot open dothinkey"); return; }
    imageGrabber->DothinkeyLoadIniFile(channel);
    if (!res) { logger->write("Cannot load dothinkey ini file"); return; }
    imageGrabber->DothinkeyStartCamera(channel);
    if (!res) { logger->write("Cannot start camera"); return;}
}

//Live Video View
void Sparrow::on_openCameraButton_clicked()
{
    logger->write("OpenDevice clicked");
    //imageGrabber->DothinkeyGrabbingThread(true);
    imageThread->start();
}

void Sparrow::on_testOpenBMP_clicked()
{
    QPixmap qp = QPixmap(":/images/Flamingo.bmp");
    if (qp.isNull()) {
        logger->write("Cannot open bmp file!");
    } else {
        //logger->write("Here!");
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(qp);
        scene->addItem(item);
        ui->testBmpView->setScene(scene);
        ui->testBmpView->show();
    }
}

void Sparrow::on_image_changed(QImage image)
{
    QPixmap pixmap = QPixmap::fromImage(image);
    QPixmap qp = QPixmap(":/images/Flamingo.bmp");
    scene->clear();
    scene->addPixmap(qp);
    ui->testBmpView->setScene(scene);
    ui->testBmpView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

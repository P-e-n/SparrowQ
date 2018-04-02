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
    imageGrabber = new Dothinkey(logger, ui->testBmpView, nullptr);
}

Sparrow::~Sparrow()
{
    delete ui;
}

void Sparrow::on_enumerateDeviceButton_clicked()
{
    const int channel = 0;
    imageGrabber->DothinkeyEnum();
    imageGrabber->DothinkeyOpen();
    imageGrabber->DothinkeyLoadIniFile(channel);
    imageGrabber->DothinkeyStartCamera(channel);
}

void Sparrow::on_openCameraButton_clicked()
{
    logger->write("OpenDevice clicked");
    imageGrabber->DothinkeyGrabbingThread(true);
}

void Sparrow::on_testOpenBMP_clicked()
{
    QPixmap qp = QPixmap(":/images/Flamingo.bmp");
    if (qp.isNull()) {
        logger->write("Cannot open bmp file!");
    } else {
        //logger->write("Here!");
        QGraphicsScene * scene = new QGraphicsScene();
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(qp);
        scene->addItem(item);
        ui->testBmpView->setScene(scene);
        ui->testBmpView->show();
    }
}

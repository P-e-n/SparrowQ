#include "sparrow.h"
#include "ui_sparrow.h"

Sparrow::Sparrow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Sparrow)
{
    setWindowIcon(QIcon(":/images/sparrow.png"));
    ui->setupUi(this);
    QString fileName = "logger.txt";
    logger = new Logger(this, fileName, ui->loggerPlainTextEdit);
    logger->write("SparrowQ Started!");
    imageGrabber = new Dothinkey(logger);
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

#include "sparrow.h"
#include "ui_sparrow.h"

Sparrow::Sparrow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Sparrow)
{
    ui->setupUi(this);

    QString fileName = "logger.txt";
    logger = new Logger(this, fileName, ui->loggerPlainTextEdit);
    logger->write("Hello Qt");
    imageGrabber = new Dothinkey(logger);
}

Sparrow::~Sparrow()
{
    delete ui;
}

void Sparrow::on_enumerateDeviceButton_clicked()
{
    imageGrabber->DothinkeyEnum();
    imageGrabber->DothinkeyOpen();
}

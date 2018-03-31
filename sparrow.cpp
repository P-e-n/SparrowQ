#include "sparrow.h"
#include "ui_sparrow.h"

#include <Windows.h>
#include <dtccm2.h>
#include <imagekit.h>

Sparrow::Sparrow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Sparrow)
{
    ui->setupUi(this);
}

Sparrow::~Sparrow()
{
    delete ui;
}

void Sparrow::on_enumerateDeviceButton_clicked()
{
    char * DeviceName[4] = {0 };
    int device_num;
    EnumerateDevice(DeviceName, 4, &device_num);
    ui->enumerateDeviceEdit->setText(QString::number(device_num));
}

#ifndef SPARROW_H
#define SPARROW_H

#include <QMainWindow>

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

private:
    Ui::Sparrow *ui;
};

#endif // SPARROW_H

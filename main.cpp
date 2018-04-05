#include "sparrow.h"
#include "logger.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    qInstallMessageHandler(sparrowLogOutput);
    //qSetMessagePattern("%{time yyyy-MM-dd hh:mm:ss.zzz} [%{type}] %{file}:%{line}(%{function}):%{message}");
    qSetMessagePattern("%{time yyyy-MM-dd hh:mm:ss.zzz} [%{type}] %{message}");
    QApplication a(argc, argv);
    qDebug("Process start!");
    Sparrow w;
    w.show();

    return a.exec();
}

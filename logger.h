#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QPlainTextEdit>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QTime>

class Logger : public QObject
{
public:
    Q_OBJECT
public:
    explicit Logger(QObject *parent, QString fileName, QPlainTextEdit *editor = 0);
    ~Logger();
    void setShowDateTime(bool value);
private:
    QFile *file;
    QPlainTextEdit *m_editor;
    bool m_showDate;
    Logger *logger;
signals:

public slots:
    void write(const QString &value);
};

#endif // LOGGER_H

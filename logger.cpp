#include "logger.h"
#include <fstream>

QPlainTextEdit * log_editor = Q_NULLPTR;

void sparrowLogOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    std::ofstream log_file("log.txt", std::ofstream::app);
    QString log_msg = qFormatLogMessage(type, context, msg);
    log_file << qPrintable(log_msg) << std::endl;
    log_file.close();
    if (log_editor) {
        log_editor->appendPlainText(log_msg);
    }
}


Logger::Logger(QObject *parent, QString fileName, QPlainTextEdit *editor) : QObject(parent) {
    m_editor = editor;
    m_showDate = true;
    if (!fileName.isEmpty()) {
        file = new QFile;
        file->setFileName(fileName);
        file->open(QIODevice::Append | QIODevice::Text);
    }
}

Logger::~Logger() {
   if (file != 0)
     file->close();
}

void Logger::write(const QString &value) {
   QString text = value;// + "";
   if (m_showDate)
   {
      text = QDateTime::currentDateTime().toString("dd.MM.yyyy ") +
             QTime::currentTime().toString("hh:mm:ss.zzz ") + text;
   }
   QTextStream out(file);
   out.setCodec("UTF-8");
   if (file != 0) {
      out << text;
   }
   if (m_editor != 0)
      m_editor->appendPlainText(text);
}

void Logger::setShowDateTime(bool value) {
   m_showDate = value;
}

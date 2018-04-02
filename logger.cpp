#include "logger.h"

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
      text = QTime::currentTime().toString("dd.MM.yyyy hh:mm:ss.zzz ") + text;
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

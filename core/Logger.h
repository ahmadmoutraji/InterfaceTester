#pragma once

#include <QDate>
#include <QFile>
#include <QMutex>
#include <QString>

class Logger
{
public:
    Logger();

    void logTestEvent(const QString &testName, const QString &message);

    QString logDirectory() const;

private:
    void ensureLogFile();
    void openLogFileForDate(const QDate &date);
    void cleanupOldLogs();
    QString resolveBaseDirectory() const;
    QString logFileNameForDate(const QDate &date) const;

    mutable QMutex m_mutex;
    QFile m_file;
    QDate m_currentDate;
    QString m_logDirectory;
};

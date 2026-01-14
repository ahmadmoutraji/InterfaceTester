#include "core/Logger.h"

#include <QDateTime>
#include <QDir>
#include <QStandardPaths>
#include <QTextStream>

namespace {
constexpr int kLogRetentionDays = 7;
constexpr const char *kLogFilePrefix = "interface-tester-";
constexpr const char *kLogFileExtension = ".log";
} // namespace

Logger::Logger()
    : m_currentDate(QDate::currentDate())
{
    m_logDirectory = resolveBaseDirectory();
    cleanupOldLogs();
}

QString Logger::logDirectory() const
{
    return m_logDirectory;
}

void Logger::logTestEvent(const QString &testName, const QString &message)
{
    QMutexLocker locker(&m_mutex);
    ensureLogFile();

    if (!m_file.isOpen()) {
        return;
    }

    QTextStream stream(&m_file);
    const QString timestamp = QDateTime::currentDateTime().toString(Qt::ISODate);
    stream << "[" << timestamp << "] "
           << "[" << testName << "] "
           << message << '\n';
    stream.flush();
}

void Logger::ensureLogFile()
{
    const QDate today = QDate::currentDate();
    if (!m_file.isOpen() || today != m_currentDate) {
        openLogFileForDate(today);
    }
}

void Logger::openLogFileForDate(const QDate &date)
{
    if (m_file.isOpen()) {
        m_file.close();
    }

    QDir dir(m_logDirectory);
    if (!dir.exists()) {
        dir.mkpath(QStringLiteral("."));
    }

    const QString filePath = dir.filePath(logFileNameForDate(date));
    m_file.setFileName(filePath);
    if (!m_file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        return;
    }
    m_currentDate = date;
}

void Logger::cleanupOldLogs()
{
    QDir dir(m_logDirectory);
    if (!dir.exists()) {
        return;
    }

    const QStringList files = dir.entryList(
        QStringList() << QStringLiteral("%1*.log").arg(kLogFilePrefix),
        QDir::Files);

    const QDate cutoff = QDate::currentDate().addDays(-kLogRetentionDays);
    for (const QString &fileName : files) {
        const QString base = fileName;
        if (!base.startsWith(kLogFilePrefix) || !base.endsWith(kLogFileExtension)) {
            continue;
        }

        const QString datePart = base.mid(
            static_cast<int>(strlen(kLogFilePrefix)),
            8);
        const QDate fileDate = QDate::fromString(datePart, "yyyyMMdd");
        if (fileDate.isValid() && fileDate < cutoff) {
            dir.remove(fileName);
        }
    }
}

QString Logger::resolveBaseDirectory() const
{
    const QString primaryDir = QStringLiteral("/var/log/interface-tester");
    const QDir primary(primaryDir);
    if (primary.exists() || primary.mkpath(QStringLiteral("."))) {
        return primaryDir;
    }

    const QString fallback = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
        + QStringLiteral("/logs");
    return fallback;
}

QString Logger::logFileNameForDate(const QDate &date) const
{
    return QStringLiteral("%1%2%3")
        .arg(kLogFilePrefix, date.toString("yyyyMMdd"), kLogFileExtension);
}

#include "tests/LanTestCase.h"

#include "core/Logger.h"

#include <QNetworkAddressEntry>
#include <QNetworkInterface>

namespace {
constexpr int kTimeoutMs = 5000;
const QString kTargetHost = QStringLiteral("google.com");
} // namespace

LanTestCase::LanTestCase(Logger *logger, QObject *parent)
    : ITestCase(parent),
      m_logger(logger)
{
    m_process.setProcessChannelMode(QProcess::MergedChannels);

    connect(&m_process, &QProcess::finished, this, &LanTestCase::handleFinished);
    connect(&m_process, &QProcess::errorOccurred, this, &LanTestCase::handleError);

    m_timeout.setSingleShot(true);
    m_timeout.setInterval(kTimeoutMs);
    connect(&m_timeout, &QTimer::timeout, this, &LanTestCase::handleTimeout);
}

QString LanTestCase::name() const
{
    return QStringLiteral("Test LAN");
}

void LanTestCase::start()
{
    if (m_process.state() != QProcess::NotRunning) {
        return;
    }

    if (m_logger) {
        m_logger->logTestEvent(name(), QStringLiteral("Checking network interfaces and pinging %1").arg(kTargetHost));
    }

    logInterfaces();

    const QString program = QStringLiteral("ping");
    const QStringList arguments = {QStringLiteral("-c"), QStringLiteral("1"),
                                   QStringLiteral("-W"), QStringLiteral("2"),
                                   kTargetHost};

    m_timeout.start();
    m_process.start(program, arguments);
}

void LanTestCase::handleFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    m_timeout.stop();

    const QString output = QString::fromLocal8Bit(m_process.readAllStandardOutput());
    if (m_logger && !output.isEmpty()) {
        m_logger->logTestEvent(name(), QStringLiteral("ping output: %1").arg(output.trimmed()));
    }

    const bool passed = (exitStatus == QProcess::NormalExit && exitCode == 0);
    finish(passed, QStringLiteral("ping exitCode=%1 exitStatus=%2")
        .arg(exitCode)
        .arg(exitStatus == QProcess::NormalExit ? QStringLiteral("Normal") : QStringLiteral("Crashed")));
}

void LanTestCase::handleError(QProcess::ProcessError error)
{
    m_timeout.stop();
    finish(false, QStringLiteral("ping process error=%1").arg(static_cast<int>(error)));
}

void LanTestCase::handleTimeout()
{
    if (m_process.state() != QProcess::NotRunning) {
        m_process.kill();
    }
    finish(false, QStringLiteral("ping timeout after %1ms").arg(kTimeoutMs));
}

void LanTestCase::logInterfaces()
{
    if (!m_logger) {
        return;
    }

    const QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface &iface : interfaces) {
        const auto flags = iface.flags();
        const bool isUp = flags.testFlag(QNetworkInterface::IsUp);
        const bool isRunning = flags.testFlag(QNetworkInterface::IsRunning);
        const bool isLoopback = flags.testFlag(QNetworkInterface::IsLoopBack);

        QStringList ipv4s;
        const QList<QNetworkAddressEntry> entries = iface.addressEntries();
        for (const QNetworkAddressEntry &entry : entries) {
            if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                ipv4s << entry.ip().toString();
            }
        }

        m_logger->logTestEvent(
            name(),
            QStringLiteral("iface=%1 up=%2 running=%3 loopback=%4 ipv4=%5")
                .arg(iface.humanReadableName(),
                     isUp ? QStringLiteral("true") : QStringLiteral("false"),
                     isRunning ? QStringLiteral("true") : QStringLiteral("false"),
                     isLoopback ? QStringLiteral("true") : QStringLiteral("false"),
                     ipv4s.isEmpty() ? QStringLiteral("none") : ipv4s.join(',')));
    }
}

void LanTestCase::finish(bool passed, const QString &reason)
{
    if (m_logger) {
        m_logger->logTestEvent(name(), reason);
    }
    emit finished(name(), passed);
}

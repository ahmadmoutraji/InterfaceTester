#include "tests/LedTestCase.h"

#include "core/Logger.h"

#include <QDir>
#include <QFile>

namespace {
constexpr int kGpioNumber = 538;
const QString kGpioBasePath = QStringLiteral("/sys/class/gpio");
const QString kDirectionOut = QStringLiteral("out");
const QString kValueHigh = QStringLiteral("1");
const QString kValueLow = QStringLiteral("0");
constexpr int kExportDelayMs = 50;
} // namespace

LedTestCase::LedTestCase(Logger *logger, QObject *parent)
    : ITestCase(parent),
      m_logger(logger),
      m_waitingForResponse(false)
{
    m_setupTimer.setSingleShot(true);
    connect(&m_setupTimer, &QTimer::timeout, this, &LedTestCase::configureGpio);
}

QString LedTestCase::name() const
{
    return QStringLiteral("Test LED");
}

void LedTestCase::start()
{
    if (m_waitingForResponse) {
        return;
    }

    m_waitingForResponse = true;
    if (m_logger) {
        m_logger->logTestEvent(name(), QStringLiteral("Starting LED test on GPIO %1").arg(kGpioNumber));
    }

    if (!ensureExported()) {
        finish(false, QStringLiteral("Failed to export GPIO %1").arg(kGpioNumber));
        return;
    }

    m_setupTimer.start(kExportDelayMs);
}

void LedTestCase::submitUserResponse(bool ledOk)
{
    if (!m_waitingForResponse) {
        return;
    }

    m_waitingForResponse = false;
    writeGpioFile(QStringLiteral("%1/gpio%2/value").arg(kGpioBasePath).arg(kGpioNumber), kValueLow.toLatin1());
    unexportGpio();
    finish(ledOk, QStringLiteral("user_response=%1").arg(ledOk ? QStringLiteral("Yes") : QStringLiteral("No")));
}

void LedTestCase::configureGpio()
{
    const QString basePath = QStringLiteral("%1/gpio%2").arg(kGpioBasePath).arg(kGpioNumber);
    const QString directionPath = basePath + QStringLiteral("/direction");
    const QString valuePath = basePath + QStringLiteral("/value");

    if (!writeGpioFile(directionPath, kDirectionOut.toLatin1())) {
        finish(false, QStringLiteral("Failed to set GPIO direction"));
        return;
    }

    if (!writeGpioFile(valuePath, kValueHigh.toLatin1())) {
        finish(false, QStringLiteral("Failed to set GPIO value high"));
        return;
    }

    if (m_logger) {
        m_logger->logTestEvent(name(), QStringLiteral("GPIO %1 set high; waiting for user confirmation")
            .arg(kGpioNumber));
    }

    emit promptRequested();
}

void LedTestCase::finish(bool passed, const QString &reason)
{
    if (m_logger) {
        m_logger->logTestEvent(name(), reason);
    }
    emit finished(name(), passed);
}

bool LedTestCase::writeGpioFile(const QString &path, const QByteArray &value)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        if (m_logger) {
            m_logger->logTestEvent(name(), QStringLiteral("write failed: %1").arg(path));
        }
        return false;
    }
    const bool ok = (file.write(value) == value.size());
    file.close();
    return ok;
}

bool LedTestCase::ensureExported()
{
    const QString gpioPath = QStringLiteral("%1/gpio%2").arg(kGpioBasePath).arg(kGpioNumber);
    if (QDir(gpioPath).exists()) {
        return true;
    }

    const QString exportPath = QStringLiteral("%1/export").arg(kGpioBasePath);
    return writeGpioFile(exportPath, QByteArray::number(kGpioNumber));
}

void LedTestCase::unexportGpio()
{
    const QString unexportPath = QStringLiteral("%1/unexport").arg(kGpioBasePath);
    writeGpioFile(unexportPath, QByteArray::number(kGpioNumber));
}

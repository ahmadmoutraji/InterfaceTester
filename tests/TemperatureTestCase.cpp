#include "tests/TemperatureTestCase.h"

#include "core/Logger.h"

#include <QDir>
#include <QFile>
#include <QTextStream>

namespace {
constexpr double kMaxIndustrialCelsius = 85.0;
const QString kThermalPath = QStringLiteral("/sys/class/thermal");
} // namespace

TemperatureTestCase::TemperatureTestCase(Logger *logger, QObject *parent)
    : ITestCase(parent),
      m_logger(logger)
{
}

QString TemperatureTestCase::name() const
{
    return QStringLiteral("Test Temperature");
}

void TemperatureTestCase::start()
{
    double valueC = 0.0;
    if (!readTemperatureCelsius(valueC)) {
        finish(false, QStringLiteral("Failed to read temperature sensors"));
        return;
    }

    if (m_logger) {
        m_logger->logTestEvent(name(), QStringLiteral("temperature_c=%1").arg(valueC, 0, 'f', 2));
        m_logger->logTestEvent(name(), QStringLiteral("threshold_c_max=%1").arg(kMaxIndustrialCelsius, 0, 'f', 2));
    }

    const bool passed = valueC <= kMaxIndustrialCelsius;
    finish(passed, QStringLiteral("temperature_c=%1 threshold_c_max=%2")
        .arg(valueC, 0, 'f', 2)
        .arg(kMaxIndustrialCelsius, 0, 'f', 2));
}

bool TemperatureTestCase::readTemperatureCelsius(double &valueC) const
{
    const QString tempPath = QStringLiteral("%1/thermal_zone0/temp").arg(kThermalPath);
    QFile tempFile(tempPath);
    if (!tempFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        if (m_logger) {
            m_logger->logTestEvent(name(), QStringLiteral("failed to open %1").arg(tempPath));
        }
        return false;
    }

    const QString raw = QString::fromLocal8Bit(tempFile.readAll()).trimmed();
    bool ok = false;
    const double value = raw.toDouble(&ok);
    if (!ok) {
        if (m_logger) {
            m_logger->logTestEvent(name(), QStringLiteral("invalid temp value: %1").arg(raw));
        }
        return false;
    }

    const double celsius = (value > 1000.0) ? value / 1000.0 : value;
    valueC = celsius;
    if (m_logger) {
        m_logger->logTestEvent(name(), QStringLiteral("sensor=thermal_zone0 raw=%1 celsius=%2")
            .arg(raw, QString::number(celsius, 'f', 2)));
    }
    return true;
}

void TemperatureTestCase::finish(bool passed, const QString &reason)
{
    if (m_logger) {
        m_logger->logTestEvent(name(), reason);
    }
    emit finished(name(), passed);
}

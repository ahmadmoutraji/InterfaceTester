#include "tests/DisplayTestCase.h"

#include "core/Logger.h"

namespace {
const QStringList kColorNames = {QStringLiteral("Red"), QStringLiteral("Green"), QStringLiteral("Blue")};
const QStringList kColorHex = {QStringLiteral("#ff0000"), QStringLiteral("#00ff00"), QStringLiteral("#0000ff")};
} // namespace

DisplayTestCase::DisplayTestCase(Logger *logger, QObject *parent)
    : ITestCase(parent),
      m_logger(logger),
      m_waitingForResponse(false),
      m_colorIndex(0)
{
}

QString DisplayTestCase::name() const
{
    return QStringLiteral("Test Display");
}

void DisplayTestCase::start()
{
    if (m_waitingForResponse) {
        return;
    }

    m_colorIndex = 0;
    m_waitingForResponse = true;
    if (m_logger) {
        m_logger->logTestEvent(name(), QStringLiteral("Starting color sequence: red, green, blue"));
    }
    requestCurrentColor();
}

void DisplayTestCase::submitUserResponse(bool sawRed)
{
    if (!m_waitingForResponse) {
        return;
    }

    const QString colorName = kColorNames.value(m_colorIndex);
    const QString response = sawRed ? QStringLiteral("Yes") : QStringLiteral("No");
    if (m_logger) {
        m_logger->logTestEvent(name(), QStringLiteral("color=%1 user_response=%2").arg(colorName, response));
    }

    if (!sawRed) {
        m_waitingForResponse = false;
        finish(false, QStringLiteral("color=%1 result=Fail").arg(colorName));
        return;
    }

    m_colorIndex++;
    if (m_colorIndex >= kColorNames.size()) {
        m_waitingForResponse = false;
        finish(true, QStringLiteral("all colors confirmed"));
        return;
    }

    requestCurrentColor();
}

void DisplayTestCase::requestCurrentColor()
{
    const QString colorName = kColorNames.value(m_colorIndex);
    const QString colorHex = kColorHex.value(m_colorIndex);
    if (m_logger) {
        m_logger->logTestEvent(name(), QStringLiteral("prompt color=%1").arg(colorName));
    }
    emit promptRequested(colorName, colorHex);
}

void DisplayTestCase::finish(bool passed, const QString &reason)
{
    if (m_logger) {
        m_logger->logTestEvent(name(), reason);
    }
    emit finished(name(), passed);
}

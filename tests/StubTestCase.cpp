#include "tests/StubTestCase.h"

#include <QTimer>

StubTestCase::StubTestCase(const QString &testName, bool defaultPass, QObject *parent)
    : ITestCase(parent),
      m_name(testName),
      m_defaultPass(defaultPass)
{
}

QString StubTestCase::name() const
{
    return m_name;
}

void StubTestCase::start()
{
    QTimer::singleShot(0, this, [this]() {
        emit finished(m_name, m_defaultPass);
    });
}

#include "TestManager.h"

#include "tests/StubTestCase.h"
#include <QDateTime>

TestManager::TestManager(QObject *parent)
    : QObject(parent)
{
    m_tests.append(new StubTestCase("Test LED", true, this));
    m_tests.append(new StubTestCase("Test LAN", true, this));
    m_tests.append(new StubTestCase("Test Temperature", true, this));
    m_tests.append(new StubTestCase("Test Display", true, this));

    for (ITestCase *test : m_tests) {
        connect(test, &ITestCase::finished, this, &TestManager::handleFinished);
    }
}

TestManager::~TestManager() = default;

QString TestManager::resultsText() const
{
    return m_resultsText;
}

void TestManager::startLedTest()
{
    runTest("Test LED");
}

void TestManager::startLanTest()
{
    runTest("Test LAN");
}

void TestManager::startTemperatureTest()
{
    runTest("Test Temperature");
}

void TestManager::startDisplayTest()
{
    runTest("Test Display");
}

void TestManager::runTest(const QString &testName)
{
    m_logger.logTestEvent(testName, QStringLiteral("Test started"));

    for (ITestCase *test : m_tests) {
        if (test->name() == testName) {
            test->start();
            return;
        }
    }
}

void TestManager::handleFinished(const QString &testName, bool passed)
{
    m_logger.logTestEvent(testName, QStringLiteral("Test finished: %1")
        .arg(passed ? QStringLiteral("Pass") : QStringLiteral("Fail")));

    const QString line = formatResultLine(testName, passed);
    if (!m_resultsText.isEmpty()) {
        m_resultsText.append('\n');
    }
    m_resultsText.append(line);
    emit resultsTextChanged();
}

QString TestManager::formatResultLine(const QString &testName, bool passed) const
{
    const QString timestamp = QDateTime::currentDateTime().toString(Qt::ISODate);
    return QString("[%1] - %2: %3")
        .arg(timestamp, testName, passed ? QStringLiteral("Pass") : QStringLiteral("Fail"));
}

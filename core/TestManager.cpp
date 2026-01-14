#include "TestManager.h"

#include "tests/DisplayTestCase.h"
#include "tests/LanTestCase.h"
#include "tests/LedTestCase.h"
#include "tests/StubTestCase.h"
#include "tests/TemperatureTestCase.h"
#include <QDateTime>

TestManager::TestManager(QObject *parent)
    : QObject(parent),
      m_displayTest(nullptr),
      m_ledTest(nullptr)
{
    m_ledTest = new LedTestCase(&m_logger, this);
    m_tests.append(m_ledTest);
    m_tests.append(new LanTestCase(&m_logger, this));
    m_tests.append(new TemperatureTestCase(&m_logger, this));
    m_displayTest = new DisplayTestCase(&m_logger, this);
    m_tests.append(m_displayTest);

    for (ITestCase *test : m_tests) {
        connect(test, &ITestCase::finished, this, &TestManager::handleFinished);
    }

    connect(m_displayTest, &DisplayTestCase::promptRequested, this, &TestManager::displayPromptRequested);
    connect(m_ledTest, &LedTestCase::promptRequested, this, &TestManager::ledPromptRequested);
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

void TestManager::submitLedResponse(bool ledOk)
{
    if (!m_ledTest) {
        return;
    }
    m_ledTest->submitUserResponse(ledOk);
}

void TestManager::submitDisplayResponse(bool sawRed)
{
    if (!m_displayTest) {
        return;
    }
    m_displayTest->submitUserResponse(sawRed);
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

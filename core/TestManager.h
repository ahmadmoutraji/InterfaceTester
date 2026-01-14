#pragma once

#include <QObject>
#include <QString>
#include <QVector>

#include "core/Logger.h"

class ITestCase;

class TestManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString resultsText READ resultsText NOTIFY resultsTextChanged)

public:
    explicit TestManager(QObject *parent = nullptr);
    ~TestManager() override;

    QString resultsText() const;

public slots:
    void startLedTest();
    void startLanTest();
    void startTemperatureTest();
    void startDisplayTest();
    void submitDisplayResponse(bool sawRed);
    void submitLedResponse(bool ledOk);

signals:
    void resultsTextChanged();
    void displayPromptRequested(const QString &colorName, const QString &colorHex);
    void ledPromptRequested();

private:
    void runTest(const QString &testName);
    void handleFinished(const QString &testName, bool passed);
    QString formatResultLine(const QString &testName, bool passed) const;

    QVector<ITestCase *> m_tests;
    QString m_resultsText;
    Logger m_logger;
    class DisplayTestCase *m_displayTest;
    class LedTestCase *m_ledTest;
};

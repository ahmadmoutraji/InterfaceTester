#pragma once

#include <QObject>
#include <QString>
#include <QVector>

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

signals:
    void resultsTextChanged();

private:
    void runTest(const QString &testName);
    void handleFinished(const QString &testName, bool passed);
    QString formatResultLine(const QString &testName, bool passed) const;

    QVector<ITestCase *> m_tests;
    QString m_resultsText;
};

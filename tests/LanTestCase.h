#pragma once

#include "core/ITestCase.h"

#include <QProcess>
#include <QTimer>

class Logger;

class LanTestCase : public ITestCase
{
    Q_OBJECT

public:
    explicit LanTestCase(Logger *logger, QObject *parent = nullptr);

    QString name() const override;

public slots:
    void start() override;

private slots:
    void handleFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void handleError(QProcess::ProcessError error);
    void handleTimeout();

private:
    void logInterfaces();
    void finish(bool passed, const QString &reason);

    Logger *m_logger;
    QProcess m_process;
    QTimer m_timeout;
};

#pragma once

#include "core/ITestCase.h"

#include <QTimer>

class Logger;

class LedTestCase : public ITestCase
{
    Q_OBJECT

public:
    explicit LedTestCase(Logger *logger, QObject *parent = nullptr);

    QString name() const override;

public slots:
    void start() override;
    void submitUserResponse(bool ledOk);

signals:
    void promptRequested();

private:
    void configureGpio();
    void finish(bool passed, const QString &reason);
    bool writeGpioFile(const QString &path, const QByteArray &value);
    bool ensureExported();
    void unexportGpio();

    Logger *m_logger;
    QTimer m_setupTimer;
    bool m_waitingForResponse;
};

#pragma once

#include "core/ITestCase.h"

class Logger;

class DisplayTestCase : public ITestCase
{
    Q_OBJECT

public:
    explicit DisplayTestCase(Logger *logger, QObject *parent = nullptr);

    QString name() const override;

public slots:
    void start() override;
    void submitUserResponse(bool sawRed);

signals:
    void promptRequested(const QString &colorName, const QString &colorHex);

private:
    void requestCurrentColor();
    void finish(bool passed, const QString &reason);

    Logger *m_logger;
    bool m_waitingForResponse;
    int m_colorIndex;
};

#pragma once

#include "core/ITestCase.h"

class Logger;

class TemperatureTestCase : public ITestCase
{
    Q_OBJECT

public:
    explicit TemperatureTestCase(Logger *logger, QObject *parent = nullptr);

    QString name() const override;

public slots:
    void start() override;

private:
    void finish(bool passed, const QString &reason);
    bool readTemperatureCelsius(double &valueC) const;

    Logger *m_logger;
};

#pragma once

#include "core/ITestCase.h"

class StubTestCase : public ITestCase
{
    Q_OBJECT

public:
    explicit StubTestCase(const QString &testName, bool defaultPass, QObject *parent = nullptr);

    QString name() const override;

public slots:
    void start() override;

private:
    QString m_name;
    bool m_defaultPass;
};

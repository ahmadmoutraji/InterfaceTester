#pragma once

#include <QObject>

class ITestCase : public QObject
{
    Q_OBJECT

public:
    explicit ITestCase(QObject *parent = nullptr) : QObject(parent) {}
    ~ITestCase() override = default;

    virtual QString name() const = 0;

public slots:
    virtual void start() = 0;

signals:
    void finished(const QString &testName, bool passed);
};

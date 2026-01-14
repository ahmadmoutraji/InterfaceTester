#pragma once

#include <QObject>
#include <QTimer>

class NetworkInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString primaryIp READ primaryIp NOTIFY primaryIpChanged)

public:
    explicit NetworkInfo(QObject *parent = nullptr);

    QString primaryIp() const;

signals:
    void primaryIpChanged();

private:
    void updatePrimaryIp();
    QString selectPrimaryIp() const;

    QTimer m_pollTimer;
    QString m_primaryIp;
};

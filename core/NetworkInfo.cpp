#include "core/NetworkInfo.h"

#include <QNetworkAddressEntry>
#include <QNetworkInterface>

namespace {
constexpr int kPollIntervalMs = 2000;
const QString kNotAvailable = QStringLiteral("Not Available");
} // namespace

NetworkInfo::NetworkInfo(QObject *parent)
    : QObject(parent),
      m_primaryIp(kNotAvailable)
{
    m_pollTimer.setInterval(kPollIntervalMs);
    connect(&m_pollTimer, &QTimer::timeout, this, &NetworkInfo::updatePrimaryIp);
    m_pollTimer.start();
    updatePrimaryIp();
}

QString NetworkInfo::primaryIp() const
{
    return m_primaryIp;
}

void NetworkInfo::updatePrimaryIp()
{
    const QString ip = selectPrimaryIp();
    if (ip == m_primaryIp) {
        return;
    }
    m_primaryIp = ip;
    emit primaryIpChanged();
}

QString NetworkInfo::selectPrimaryIp() const
{
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    std::sort(interfaces.begin(), interfaces.end(), [](const QNetworkInterface &a, const QNetworkInterface &b) {
        return a.humanReadableName().toLower() < b.humanReadableName().toLower();
    });

    for (const QNetworkInterface &iface : interfaces) {
        const auto flags = iface.flags();
        if (!(flags & QNetworkInterface::IsUp) || !(flags & QNetworkInterface::IsRunning)) {
            continue;
        }
        if (flags & QNetworkInterface::IsLoopBack) {
            continue;
        }

        const QList<QNetworkAddressEntry> entries = iface.addressEntries();
        for (const QNetworkAddressEntry &entry : entries) {
            if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                return entry.ip().toString();
            }
        }
    }

    return kNotAvailable;
}

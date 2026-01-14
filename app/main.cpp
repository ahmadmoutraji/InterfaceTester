#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

#include "core/TestManager.h"
#include "core/NetworkInfo.h"

int main(int argc, char *argv[])
{
    QQuickStyle::setStyle("Basic");

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    TestManager testManager;
    NetworkInfo networkInfo;
    engine.rootContext()->setContextProperty("testManager", &testManager);
    engine.rootContext()->setContextProperty("networkInfo", &networkInfo);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("InterfaceTester", "Main");

    return app.exec();
}

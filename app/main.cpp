#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

#include "core/TestManager.h"

int main(int argc, char *argv[])
{
    QQuickStyle::setStyle("Basic");

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    TestManager testManager;
    engine.rootContext()->setContextProperty("testManager", &testManager);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("InterfaceTester", "Main");

    return app.exec();
}

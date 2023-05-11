#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "TwController.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    app.setOrganizationName("DLN");
    app.setOrganizationDomain("com.dln");

    // create objects
    TwController twController;
    QQmlApplicationEngine engine; // create objects before the engine!

    // set context properties
    engine.rootContext()->setContextProperty("_twController", &twController);

    // load main.qml
    const QUrl url(QStringLiteral("qrc:/resources/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}

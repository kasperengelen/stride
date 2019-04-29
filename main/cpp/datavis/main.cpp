#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <iostream>

#include "datavis/controller/Controller.h"

int main(int argc, char *argv[])
{

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    // register Controller QML element
    qmlRegisterType<stride::datavis::Controller>("stride.datavis.controller", 1, 0, "Controller");

    // register MapView QML element
    //qmlRegisterType<stride::datavis::MapView>("stride.datavis.mapview", 1, 0, "MapView");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    // retrieve Map and manipulate it

    return app.exec();
}

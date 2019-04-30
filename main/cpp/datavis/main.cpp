#include <QApplication>
#include <QQmlApplicationEngine>
#include <QWindow>

#include <iostream>

#include "datavis/controller/Controller.h"
#include "datavis/model/Model.h"

using stride::datavis::Controller;
using stride::datavis::Model;

int main(int argc, char *argv[])
{

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    // register QML elements
    qmlRegisterType<Controller>("stride.datavis.controller", 1, 0, "Controller");
    qmlRegisterType<Model>("stride.datavis.model", 1, 0, "Model");

    // create engine
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    // get controller
    Controller* ctrl_ptr = engine.rootObjects().at(0)->findChild<Controller*>("controller");

    // connect controller with model
    Model* model_ptr = engine.rootObjects().at(0)->findChild<Model*>("model");
    ctrl_ptr->SetModelPointer(model_ptr);

    // set window_ptr
    QWindow* window_ptr = dynamic_cast<QWindow*>(engine.rootObjects().at(0));



    return app.exec();
}

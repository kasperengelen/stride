#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QWindow>

#include <iostream>

#include "visualiser/controller/Controller.h"
#include "visualiser/model/Model.h"
#include "visualiser/view/View.h"

using stride::visualiser::Controller;
using stride::visualiser::Model;
using stride::visualiser::View;

/**
 * Main method.
 */
int main(int argc, char* argv[])
{

        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
        QApplication app(argc, argv);

        // register QML elements
        qmlRegisterType<Controller>("stride.visualiser.controller", 1, 0, "Controller");
        qmlRegisterType<View>("stride.visualiser.view", 1, 0, "View");

        // create engine
        QQmlApplicationEngine engine;
        engine.load(QUrl(QStringLiteral("qrc:/view/main.qml")));
        if (engine.rootObjects().isEmpty())
                return -1;

        // get controller
        Controller* ctrl_ptr = engine.rootObjects().at(0)->findChild<Controller*>("controller");

        // get view
        View* view_ptr = engine.rootObjects().at(0)->findChild<View*>("view");

        // connect controller to view.
        ctrl_ptr->SetViewPointer(view_ptr);

        // create model
        Model model{};

        ctrl_ptr->SetModelPointer(&model);
        view_ptr->SetModelPointer(&model);

        return app.exec();
}

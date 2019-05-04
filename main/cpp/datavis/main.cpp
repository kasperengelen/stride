#include <QApplication>
#include <QQmlApplicationEngine>
#include <QWindow>

#include <iostream>

#include "datavis/controller/Controller.h"
#include "datavis/view/View.h"
#include "datavis/model/Model.h"

using stride::datavis::Controller;
using stride::datavis::Model;
using stride::datavis::View;

int main(int argc, char *argv[]) {

	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QApplication app(argc, argv);

	// register QML elements
	qmlRegisterType<Controller>("stride.datavis.controller", 1, 0,
			"Controller");
	qmlRegisterType<View>("stride.datavis.view", 1, 0, "View");

	// create engine
	QQmlApplicationEngine engine;
	engine.load(QUrl(QStringLiteral("qrc:/view/main.qml")));
	if (engine.rootObjects().isEmpty())
		return -1;

	// get controller
	Controller* ctrl_ptr = engine.rootObjects().at(0)->findChild<Controller*>(
			"controller");

	// get view
	View* view_ptr = engine.rootObjects().at(0)->findChild<View*>("view");

	// create model
	Model model { };
	ctrl_ptr->SetModelPointer(&model);
	view_ptr->SetModelPointer(&model);

	return app.exec();
}

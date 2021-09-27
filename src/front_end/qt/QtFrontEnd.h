//
// Created by thallock on 9/15/21.
//

#ifndef GLVIEW_QTFRONTEND_H
#define GLVIEW_QTFRONTEND_H


#include <QOpenGLWidget>
#include <QApplication>

#include "./ProjectorWidget.h"
#include "../FrontEnd.h"


namespace billiards::qt {

	class QtFrontEnd : public billiards::project::FrontEnd {
	public:
		QApplication app;
		ProjectorWidget projector;

		QtFrontEnd(int& argc, char **argv)
			: FrontEnd{}
			, app{argc, argv}
			, projector{display}
			{}

		int loop() override {
			projector.show();
//			projector.showFullScreen();
			return app.exec();
		}

		void redraw() override {
			projector.update();
		}

		void shutdown() override {
			app.quit();
		}
	};
}


#endif //GLVIEW_QTFRONTEND_H

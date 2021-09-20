//
// Created by thallock on 9/15/21.
//

#ifndef GLVIEW_PROJECTORWIDGET_H
#define GLVIEW_PROJECTORWIDGET_H

#include <QOpenGLWidget>
#include <QWidget>


#include "../../DisplaySettings.h"


namespace billiards::qt {

	class ProjectorWidget : public QOpenGLWidget {
		Q_OBJECT

	public:
		explicit ProjectorWidget(billiards::project::DisplaySettings& settings, QWidget *parent = 0);

	protected:
		void paintEvent(QPaintEvent *);

	private:
		billiards::project::DisplaySettings& display;
		QFont font;
	};

}


#endif //GLVIEW_PROJECTORWIDGET_H

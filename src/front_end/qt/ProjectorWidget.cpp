//
// Created by thallock on 9/15/21.
//

#include "ProjectorWidget.h"

#include <QtCore/QElapsedTimer>
#include <QtGui/QPainter>
#include <QtWidgets/QApplication>
#include <QPainterPath>

#include "./paint.h"

namespace billiards::qt {

	ProjectorWidget::ProjectorWidget(billiards::project::DisplaySettings& display, QWidget *parent)
		: QOpenGLWidget{parent}, display{display}, font{"DejaVu Sans Mono", 16} {}


	void ProjectorWidget::paintEvent(QPaintEvent *) {
		QPainter painter(this);
		QBrush brush;
		brush.setStyle(Qt::SolidPattern);

		int w = width();
		int h = height();
		painter.fillRect(0, 0, w, h, Qt::black);

		std::shared_ptr<gphx::Circle> ptr = std::make_shared<gphx::Circle>(geometry::Point{25, 25}, 25);
		ptr->color = gphx::Color{1, 1, 0, 1};
		ptr->fill = true;
		paint(display.location, painter, ptr);

		painter.setFont(font);
		painter.setPen(Qt::green);
		painter.drawText(10, 100, QString("Here, we have some text"));
	}
}
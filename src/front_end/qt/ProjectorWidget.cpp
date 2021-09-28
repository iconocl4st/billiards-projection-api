//
// Created by thallock on 9/15/21.
//

#include "ProjectorWidget.h"

#include <QtCore/QElapsedTimer>
#include <QtGui/QPainter>
#include <QtWidgets/QApplication>
#include <QPainterPath>

#include "./paint.h"
#include "QtReceiver.h"

namespace billiards::qt {

	ProjectorWidget::ProjectorWidget(billiards::project::DisplaySettings& display, QWidget *parent)
		: QOpenGLWidget{parent}, display{display}, font{"DejaVu Sans Mono", 16} {}


	void ProjectorWidget::paintEvent(QPaintEvent *) {
		std::lock_guard<std::mutex> guard{display.graphics.mutex};

		QPainter painter(this);
		QBrush brush;
		brush.setStyle(Qt::SolidPattern);

		int w = width();
		int h = height();
		painter.fillRect(0, 0, w, h, Qt::black);

//		std::shared_ptr<gphx::Circle> ptr = std::make_shared<gphx::Circle>(geometry::Point{25, 25}, 25);
//		ptr->color = gphx::Color{1, 1, 0, 1};
//		ptr->fill = true;
//		paint(display.location, painter, ptr);

		painter.setFont(font);
		painter.setPen(Qt::green);
		painter.drawText(10, 100, QString("No graphics to display."));


		Receiver receiver{display.location, painter};
		for (const auto& graphics : display.graphics.current_graphics) {
			receiver.accept(graphics.get());
		}

		{
//				geometry::Point{},
			graphics::Lines edges;
			edges.line_width = 1;
			edges.fill = false;
			edges.color = graphics::Color{0, 0, 255, 255};
			edges.points.emplace_back(0, 0);
			edges.points.emplace_back(display.location.table_width, 0);
			edges.points.emplace_back(display.location.table_width, display.location.table_height);
			edges.points.emplace_back(0, display.location.table_height);
			edges.points.emplace_back(0, 0);
			receiver.accept(&edges);
		}

//		receiver.fill_segment(
//			graphics::Color{0, 0, 1, 1},
//			5,
//			geometry::Point{5, 15},
//			geometry::Point{40, 10},
//			true, true
//		);
	}
}
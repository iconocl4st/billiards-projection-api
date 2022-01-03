//
// Created by thallock on 9/15/21.
//

#include "ProjectorWidget.h"

#include <QtCore/QElapsedTimer>
#include <QtGui/QPainter>
#include <QtWidgets/QApplication>
#include <QPainterPath>

#include "front_end/qt/paint.h"
#include "front_end/qt/QtReceiver.h"

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

		if (display.graphics.current_graphics.empty()) {
			painter.setFont(font);
			painter.setPen(Qt::green);
			painter.drawText(w / 2, h / 2, QString("No graphics to display."));
		}

		Receiver receiver{display.map, painter};
		for (const auto& graphics : display.graphics.current_graphics) {
			receiver.accept(graphics.get());
		}
	}
}